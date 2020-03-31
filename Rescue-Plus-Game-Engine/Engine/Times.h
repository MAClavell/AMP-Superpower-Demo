#pragma once
class Time
{
private:
	static float m_deltaTime;
	static float m_fixedDeltaTime;
	static float m_totalTime;

	Time() {};
	~Time() {};

public:
	// --------------------------------------------------------
	// Get the delta time of this frame
	// --------------------------------------------------------
	static float deltaTime() { return m_deltaTime; };

	// --------------------------------------------------------
	// Get the fixed delta time of this frame
	// --------------------------------------------------------
	static float fixedDeltaTime() { return m_fixedDeltaTime; };

	// --------------------------------------------------------
	// Get the total time this application was running
	// --------------------------------------------------------
	static float totalTime() { return m_totalTime; };

	// --------------------------------------------------------
	// FOR ENGINE USE ONLY!
	// DO NOT SET DELTA TIME MANUALLY!
	// --------------------------------------------------------
	static void setDeltaTime(float newDeltaTime) { m_deltaTime = newDeltaTime; };

	// --------------------------------------------------------
	// FOR ENGINE USE ONLY!
	// DO NOT SET FIXED DELTA TIME MANUALLY!
	// --------------------------------------------------------
	static void setFixedDeltaTime(float newFixedDeltaTime) { m_fixedDeltaTime = newFixedDeltaTime; };

	// --------------------------------------------------------
	// FOR ENGINE USE ONLY!
	// DO NOT SET TOTAL TIME MANUALLY!
	// --------------------------------------------------------
	static void setTotalTime(float newTotalTime) { m_totalTime = newTotalTime; };
};

