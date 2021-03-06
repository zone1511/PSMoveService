#ifndef CLIENT_HMD_VIEW_H
#define CLIENT_HMD_VIEW_H

//-- includes -----
#include "PSMoveClient_export.h"
#include "ClientConstants.h"
#include "ClientGeometry.h"
#include <cassert>

//-- pre-declarations -----
namespace PSMoveProtocol
{
    class DeviceOutputDataFrame;
    class DeviceOutputDataFrame_HMDDataPacket;
};

//-- constants -----

//-- declarations -----
struct PSM_CPP_PUBLIC_CLASS MorpheusPhysicsData
{
	PSMoveFloatVector3 VelocityCmPerSec;
	PSMoveFloatVector3 AccelerationCmPerSecSqr;
	PSMoveFloatVector3 AngularVelocityRadPerSec;
	PSMoveFloatVector3 AngularAccelerationRadPerSecSqr;

	inline void Clear()
	{
		VelocityCmPerSec = *k_psmove_float_vector3_zero;
		AccelerationCmPerSecSqr = *k_psmove_float_vector3_zero;
		AngularVelocityRadPerSec = *k_psmove_float_vector3_zero;
		AngularAccelerationRadPerSecSqr = *k_psmove_float_vector3_zero;
	}
};

struct PSM_CPP_PUBLIC_CLASS MorpheusRawSensorData
{
	PSMoveIntVector3 Accelerometer;
	PSMoveIntVector3 Gyroscope;

    inline void Clear()
    {
        Accelerometer = *k_psmove_int_vector3_zero;
        Gyroscope = *k_psmove_int_vector3_zero;
    }
};

struct PSM_CPP_PUBLIC_CLASS MorpheusCalibratedSensorData
{
	PSMoveFloatVector3 Accelerometer;
	PSMoveFloatVector3 Gyroscope;

	inline void Clear()
	{
		Accelerometer = *k_psmove_float_vector3_zero;
		Gyroscope = *k_psmove_float_vector3_zero;
	}
};

struct PSM_CPP_PUBLIC_CLASS MorpheusRawTrackerData
{
	// Parallel arrays: ScreenLocations, Positions and the TrackerID associated with them
	PSMoveScreenLocation ScreenLocations[PSMOVESERVICE_MAX_TRACKER_COUNT];
	PSMovePosition RelativePositionsCm[PSMOVESERVICE_MAX_TRACKER_COUNT];
	PSMoveQuaternion RelativeOrientations[PSMOVESERVICE_MAX_TRACKER_COUNT];
	PSMoveTrackingProjection TrackingProjections[PSMOVESERVICE_MAX_TRACKER_COUNT];
	int TrackerIDs[PSMOVESERVICE_MAX_TRACKER_COUNT];
	int ValidTrackerLocations;

	inline void Clear()
	{
		for (int index = 0; index < PSMOVESERVICE_MAX_TRACKER_COUNT; ++index)
		{
			ScreenLocations[index] = PSMoveScreenLocation::create(0, 0);
			RelativePositionsCm[index] = *k_psmove_position_origin;
			RelativeOrientations[index] = *k_psmove_quaternion_identity;
			TrackerIDs[index] = -1;
		}
		ValidTrackerLocations = 0;
	}

	inline bool GetPixelLocationOnTrackerId(int trackerId, PSMoveScreenLocation &outLocation) const
	{
		bool bFound = false;

		for (int listIndex = 0; listIndex < ValidTrackerLocations; ++listIndex)
		{
			if (TrackerIDs[listIndex] == trackerId)
			{
				outLocation = ScreenLocations[listIndex];
				bFound = true;
				break;
			}
		}

		return bFound;
	}

	inline bool GetPositionOnTrackerId(int trackerId, PSMovePosition &outPosition) const
	{
		bool bFound = false;

		for (int listIndex = 0; listIndex < ValidTrackerLocations; ++listIndex)
		{
			if (TrackerIDs[listIndex] == trackerId)
			{
				outPosition = RelativePositionsCm[listIndex];
				bFound = true;
				break;
			}
		}

		return bFound;
	}

	inline bool GetOrientationOnTrackerId(int trackerId, PSMoveQuaternion &outOrientation) const
	{
		bool bFound = false;

		for (int listIndex = 0; listIndex < ValidTrackerLocations; ++listIndex)
		{
			if (TrackerIDs[listIndex] == trackerId)
			{
				outOrientation = RelativeOrientations[listIndex];
				bFound = true;
				break;
			}
		}

		return bFound;
	}

	inline bool GetProjectionOnTrackerId(int trackerId, PSMoveTrackingProjection &outProjection) const
	{
		bool bFound = false;

		for (int listIndex = 0; listIndex < ValidTrackerLocations; ++listIndex)
		{
			if (TrackerIDs[listIndex] == trackerId)
			{
				outProjection = TrackingProjections[listIndex];
				bFound = true;
				break;
			}
		}

		return bFound;
	}
};

struct PSM_CPP_PUBLIC_CLASS ClientMorpheusView
{
private:
	bool bValid;
	bool bIsTrackingEnabled;
	bool bIsCurrentlyTracking;
	bool bIsOrientationValid;
	bool bIsPositionValid;

	PSMovePose Pose;
	MorpheusPhysicsData PhysicsData;
	MorpheusRawSensorData RawSensorData;
	MorpheusCalibratedSensorData CalibratedSensorData;
	MorpheusRawTrackerData RawTrackerData;

public:
    void Clear();
    void ApplyHMDDataFrame(const PSMoveProtocol::DeviceOutputDataFrame_HMDDataPacket *data_frame);

    inline bool IsValid() const
    {
        return bValid;
    }

    inline void SetValid(bool flag)
    {
        bValid = flag;
    }

	inline bool GetIsCurrentlyTracking() const
	{
		return IsValid() ? bIsCurrentlyTracking : false;
	}

	inline bool GetIsTrackingEnabled() const
	{
		return IsValid() ? bIsTrackingEnabled : false;
	}

	inline bool GetIsOrientationValid() const
	{
		return IsValid() ? bIsOrientationValid : false;
	}

	inline bool GetIsPositionValid() const
	{
		return IsValid() ? bIsPositionValid : false;
	}

	inline const PSMovePose &GetPose() const
	{
		return IsValid() ? Pose : *k_psmove_pose_identity;
	}

	inline const PSMovePosition &GetPosition() const
	{
		return IsValid() ? Pose.Position : *k_psmove_position_origin;
	}

	inline const PSMoveQuaternion &GetOrientation() const
	{
		return IsValid() ? Pose.Orientation : *k_psmove_quaternion_identity;
	}

	const MorpheusPhysicsData &GetPhysicsData() const;
	const MorpheusRawSensorData &GetRawSensorData() const;
	const MorpheusCalibratedSensorData &GetCalibratedSensorData() const;
	const MorpheusRawTrackerData &GetRawTrackerData() const;
	bool GetIsStable() const;
	bool GetIsStableAndAlignedWithGravity() const;
};

class PSM_CPP_PUBLIC_CLASS ClientHMDView
{
public:
    enum eHMDViewType
    {
        None = -1,
        Morpheus,
    };

private:

	union
    {
        ClientMorpheusView MorpheusView;
    } ViewState;
    eHMDViewType HMDViewType;

    int HmdID;
    int SequenceNum;
    int ListenerCount;

    bool IsConnected;

    long long data_frame_last_received_time;
    float data_frame_average_fps;

public:
    ClientHMDView(int ControllerID);

    void Clear();
    void ApplyHMDDataFrame(const PSMoveProtocol::DeviceOutputDataFrame_HMDDataPacket *data_frame);

    // Listener State
    inline void IncListenerCount()
    {
        ++ListenerCount;
    }

    inline void DecListenerCount()
    {
        assert(ListenerCount > 0);
        --ListenerCount;
    }

    inline int GetListenerCount() const
    {
        return ListenerCount;
    }

    // HMD Data Accessors
    inline int GetHmdID() const
    {
        return HmdID;
    }

    inline int GetSequenceNum() const
    {
        return IsValid() ? SequenceNum : -1;
    }

    inline eHMDViewType GetHmdViewType() const
    {
        return HMDViewType;
    }

    inline PSMovePose GetHmdPose() const
    {
		PSMovePose result= PSMovePose::identity();

		switch (HMDViewType)
		{
		case Morpheus:
			result= GetMorpheusView().GetPose();
			break;
		}

        return result;
    }

    inline const ClientMorpheusView &GetMorpheusView() const
    {
        assert(HMDViewType == Morpheus);
        return ViewState.MorpheusView;
    }

    inline bool IsValid() const
    {
        return HmdID != -1;
    }

    inline bool GetIsConnected() const
    {
        return (IsValid() && IsConnected);
    }

	const PSMovePose &GetPose() const;
	const PSMovePosition &GetPosition() const;
	const PSMoveQuaternion &GetOrientation() const;
	const MorpheusPhysicsData &GetPhysicsData() const;
	const MorpheusRawTrackerData &GetRawTrackerData() const;

	bool GetIsCurrentlyTracking() const;
	bool GetIsPoseValid() const;
	bool GetIsStable() const;

    // Statistics
    inline float GetDataFrameFPS() const
    {
        return data_frame_average_fps;
    }
};
#endif // CLIENT_HMD_VIEW_H
