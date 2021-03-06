﻿#pragma once
#include <Limelight.h>
#include <string.h>

typedef unsigned char byte;

namespace Limelight_common_binding
{
	public ref class LimelightStreamConfiguration sealed
	{
	public:
		LimelightStreamConfiguration(int width, int height, int fps, int bitrate, int packetSize,
			const Platform::Array<unsigned char> ^riAesKey, const Platform::Array<unsigned char> ^riAesIv) :
			m_Width(width), m_Height(height), m_Fps(fps), m_Bitrate(bitrate), m_PacketSize(packetSize)
		{
			memcpy(m_riAesKey, riAesKey->Data, sizeof(m_riAesKey));
			memcpy(m_riAesIv, riAesIv->Data, sizeof(m_riAesIv));
		}

		int GetWidth(void) {
			return m_Width;
		}
		int GetHeight(void) {
			return m_Height;
		}
		int GetFps(void) {
			return m_Fps;
		}
		int GetBitrate(void) {
			return m_Bitrate;
		}
		int GetPacketSize(void) {
			return m_PacketSize;
		}
		Platform::Array<unsigned char>^ GetRiAesKey(void) {
			return ref new Platform::Array<byte>(m_riAesKey, sizeof(m_riAesKey));
		}
		Platform::Array<unsigned char>^ GetRiAesIv(void) {
			return ref new Platform::Array<byte>(m_riAesIv, sizeof(m_riAesIv));
		}

	private:
		int m_Width;
		int m_Height;
		int m_Fps;
		int m_Bitrate;
		int m_PacketSize;
		byte m_riAesKey[16];
		byte m_riAesIv[16];
	};

	public delegate void DrSetup(int width, int height, int redrawRate, int drFlags);
	public delegate void DrStart(void);
	public delegate void DrStop(void);
	public delegate void DrRelease(void);
	public delegate void DrSubmitDecodeUnit(const Platform::Array<unsigned char> ^data);

	public ref class LimelightDecoderRenderer sealed
	{
	public:
		LimelightDecoderRenderer(DrSetup ^drSetup, DrStart ^drStart, DrStop ^drStop,
			DrRelease ^drRelease, DrSubmitDecodeUnit ^drSubmitDecodeUnit) :
			m_DrSetup(drSetup), m_DrStart(drStart), m_DrStop(drStop),
			m_DrRelease(drRelease), m_DrSubmitDecodeUnit(drSubmitDecodeUnit) {}

		void Setup(int width, int height, int redrawRate, int drFlags) {
			m_DrSetup(width, height, redrawRate, drFlags);
		}
		void Start(void) {
			m_DrStart();
		}
		void Stop(void) {
			m_DrStop();
		}
		void Destroy(void) {
			m_DrRelease();
		}
		void SubmitDecodeUnit(const Platform::Array<byte> ^dataArray) {
			m_DrSubmitDecodeUnit(dataArray);
		}

	private:
		Limelight_common_binding::DrSetup ^m_DrSetup;
		Limelight_common_binding::DrStart ^m_DrStart;
		Limelight_common_binding::DrStop ^m_DrStop;
		Limelight_common_binding::DrRelease ^m_DrRelease;
		Limelight_common_binding::DrSubmitDecodeUnit ^m_DrSubmitDecodeUnit;
	};

	public delegate void ArInit(void);
	public delegate void ArStart(void);
	public delegate void ArStop(void);
	public delegate void ArRelease(void);
	public delegate void ArPlaySample(const Platform::Array<unsigned char> ^data);

	public ref class LimelightAudioRenderer sealed
	{
	public:
		LimelightAudioRenderer(ArInit ^arInit, ArStart ^arStart, ArStop ^arStop,
			ArRelease ^arRelease, ArPlaySample ^arPlaySample) :
			m_ArInit(arInit), m_ArStart(arStart), m_ArStop(arStop), m_ArRelease(arRelease),
			m_ArPlaySample(arPlaySample) {}

		void Init(void) {
			m_ArInit();
		}
		void Start(void) {
			m_ArStart();
		}
		void Stop(void) {
			m_ArStop();
		}
		void Destroy(void) {
			m_ArRelease();
		}
		void PlaySample(const Platform::Array<byte> ^dataArray) {
			m_ArPlaySample(dataArray);
		}

	private:
		Limelight_common_binding::ArInit ^m_ArInit;
		Limelight_common_binding::ArStart ^m_ArStart;
		Limelight_common_binding::ArStop ^m_ArStop;
		Limelight_common_binding::ArRelease ^m_ArRelease;
		Limelight_common_binding::ArPlaySample ^m_ArPlaySample;
	};

	public delegate void PlThreadStart(void);
	public delegate void PlDebugPrint(Platform::String ^message);

	public ref class LimelightPlatformCallbacks sealed
	{
	public:
		LimelightPlatformCallbacks(PlThreadStart ^plThreadStart, PlDebugPrint ^plDebugPrint) :
			m_PlThreadStart(plThreadStart), m_PlDebugPrint(plDebugPrint) {}

		void ThreadStart(void) {
			m_PlThreadStart();
		}

		void DebugPrint(Platform::String ^message) {
			m_PlDebugPrint(message);
		}

	private:
		Limelight_common_binding::PlThreadStart ^m_PlThreadStart;
		Limelight_common_binding::PlDebugPrint ^m_PlDebugPrint;
	};

	public delegate void ClStageStarting(int stage);
	public delegate void ClStageComplete(int stage);
	public delegate void ClStageFailed(int stage, int errorCode);
	public delegate void ClConnectionStarted(void);
	public delegate void ClConnectionTerminated(int errorCode);
	public delegate void ClDisplayMessage(Platform::String ^message);
	public delegate void ClDisplayTransientMessage(Platform::String ^message);

	public ref class LimelightConnectionListener sealed
	{
	public:
		LimelightConnectionListener(ClStageStarting ^clStageStarting, ClStageComplete ^clStageComplete,
			ClStageFailed ^clStageFailed, ClConnectionStarted ^clConnectionStarted,
			ClConnectionTerminated ^clConnectionTerminated, ClDisplayMessage ^clDisplayMessage,
			ClDisplayTransientMessage ^clDisplayTransientMessage) : m_ClStageStarting(clStageStarting),
			m_ClStageComplete(clStageComplete), m_ClStageFailed(clStageFailed), m_ClConnectionStarted(clConnectionStarted),
			m_ClConnectionTerminated(clConnectionTerminated), m_ClDisplayMessage(clDisplayMessage),
			m_ClDisplayTransientMessage(clDisplayTransientMessage) {}

		void StageStarting(int stage) {
			m_ClStageStarting(stage);
		}
		void StageComplete(int stage) {
			m_ClStageComplete(stage);
		}
		void StageFailed(int stage, int errorCode) {
			m_ClStageFailed(stage, errorCode);
		}
		void ConnectionStarted(void) {
			m_ClConnectionStarted();
		}
		void ConnectionTerminated(int errorCode) {
			m_ClConnectionTerminated(errorCode);
		}
		void DisplayMessage(Platform::String ^message) {
			m_ClDisplayMessage(message);
		}
		void DisplayTransientMessage(Platform::String ^message) {
			m_ClDisplayTransientMessage(message);
		}

	private:
		Limelight_common_binding::ClStageStarting ^m_ClStageStarting;
		Limelight_common_binding::ClStageComplete ^m_ClStageComplete;
		Limelight_common_binding::ClStageFailed ^m_ClStageFailed;
		Limelight_common_binding::ClConnectionStarted ^m_ClConnectionStarted;
		Limelight_common_binding::ClConnectionTerminated ^m_ClConnectionTerminated;
		Limelight_common_binding::ClDisplayMessage ^m_ClDisplayMessage;
		Limelight_common_binding::ClDisplayTransientMessage ^m_ClDisplayTransientMessage;
	};

	public enum class MouseButtonAction : int {
		Press = 0x07,
		Release = 0x08
	};

	public enum class MouseButton : int {
		Left = 0x01,
		Middle = 0x02,
		Right = 0x03
	};

	public enum class KeyAction : int {
		Down = 0x03,
		Up = 0x04
	};

	public enum class Modifier : int {
		ModifierShift = 0x01,
		ModifierCtrl = 0x02,
		ModifierAlt = 0x04
	};

	public enum class ButtonFlags : int {
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000,
		Up = 0x0001,
		Down = 0x0002,
		Left = 0x0004,
		Right = 0x0008,
		LB = 0x0100,
		RB = 0x0200,
		Play = 0x0010,
		Back = 0x0020,
		LS = 0x0040,
		RS = 0x0080,
		Special = 0x0400
	};

	public ref class LimelightCommonRuntimeComponent sealed
	{
	public:
		static int StartConnection(unsigned int hostAddress, LimelightStreamConfiguration ^streamConfig,
			LimelightConnectionListener ^clCallbacks, LimelightDecoderRenderer ^drCallbacks, LimelightAudioRenderer ^arCallbacks,
			LimelightPlatformCallbacks ^plCallbacks);

		static void StopConnection(void);
		static int SendMouseMoveEvent(short deltaX, short deltaY);
		static int SendMouseButtonEvent(unsigned char action, int button);
		static int SendKeyboardEvent(short keyCode, unsigned char keyAction, unsigned char modifiers);
		static int SendControllerInput(short buttonFlags, byte leftTrigger, byte rightTrigger, short leftStickX, 
			short leftStickY, short rightStickX, short rightStickY);

		// Platform-specific code
		static void CompleteThreadStart(void);
	};
}