#pragma once


class States
{
public:
	static bool GetPausedState();
	static void SetPausedState(bool bState);

private:
	static bool bPaused;
};

