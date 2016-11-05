#ifndef INPUT_H
#define INPUT_H

class CInput
{
public:
	CInput();
	CInput(const CInput&);
	~CInput();

	// Initialised the input object class.
	void Initialise();

	// Triggered when key is pressed down. 
	void KeyDown(unsigned int input);
	// Triggered when key is let go of.
	void KeyUp(unsigned int input);

	// Is the key being held down?
	bool IsKeyDown(unsigned int key);

	bool KeyHeld(unsigned int key);

	bool KeyHit(unsigned int key);

private:
	bool mKeys[256];

	// Track whether a key was held last frame.
	bool mWasHeldLastFrame[256];
};

#endif