#ifndef PRIOENGINEVARS_H
#define PRIOENGINEVARS_H

#include "Logger.h"
#include <windows.h>

extern CLogger* gLogger;

namespace PrioEngine
{
	const float kPi = 3.14159265359f;

	// Defines 3 floats for use with 3D coordinates.
	typedef struct Coords
	{
		float x;
		float y;
		float z;
	} coordinates;

	// Defines 3 floats for describing red, green and blue values.
	typedef struct RGB
	{
		float r;
		float g;
		float b;
	} RedGreenBlue;

	// Defines 4 floats for describing red, green, blue and alpha values.
	typedef struct RGBA
	{
		float r;
		float g;
		float b;
		float a;
	} RedGreenBlueAlpha;

	// An enumeration of types of shapes which we should be able to create.
	enum Primitives
	{
		triangle,
		square,
		cube,
		sphere
	};

	namespace Cube
	{
		// Store the number of vertices for a cube.
		const int kNumOfVertices = 8;
		// Store the number of indices for a cube.
		const int kNumOfIndices = 36;

		// Store the position of each vertex.
		const Coords kCubeVerticesCoords[kNumOfVertices] = {
			{-0.5f, 0.5f, -0.5f},
			{0.5f, 0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, -0.5f},
			{-0.5f, 0.5f, 0.5f},
			{0.5f, 0.5f, 0.5f},
			{-0.5f, -0.5f, 0.5f},
			{0.5f, -0.5f, 0.5f}
		};

		// Indice array which will describe which vertex's connect to make triangles.
		const unsigned long indices[kNumOfIndices] =
		{
			0, 1, 2,
			2, 1, 3,
			4, 0, 6,
			6, 0, 2,
			7, 5, 6,
			6, 5, 4,
			3, 1, 7,
			7, 1, 5,
			4, 5, 0,
			0, 5, 1,
			3, 7, 2,
			2, 7, 6
		};
	}

	namespace Triangle
	{
		// Define how many points (vertices) there will be in a triangle.
		const int kNumOfVertices = 3;
		// Define how many points will need to be connected to create triangles to form this shape.
		const int kNumOfIndices = 3;

		// The position of the points.
		const Coords vertices[kNumOfVertices] =
		{
			{-0.5f, -0.5f, 0.0f},
			{0.0f, 0.5f, 0.0f},
			{0.5f, -0.5f, 0.0f}
		};

		// The points which need to be connected together to create triangles.
		const unsigned long indices[kNumOfIndices] =
		{
			1, 2, 0
		};

	}

	// Store all default colours that can be used in the engine in this namespace so they can be accessed by PrioEngine::Colours::
	namespace Colours
	{
		// Define the colour to be used as diffuse lighting.
		const RGBA white = { 1.0f, 1.0f, 1.0f, 1.0f};
		const RGBA red = { 1.0f, 0.0f, 0.0f, 1.0f };
		const RGBA black = { 0.0f, 0.0f, 0.0f, 1.0f };
		const RGBA green = { 0.0f, 1.0f, 0.0f, 1.0f };
		const RGBA blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	}

	// Defines the types of vertex arrays we have, these should reflect the structures in the vertex shader.
	enum VertexType
	{
		Colour,
		Texture,
		Diffuse
	};

	// A namespace containing any keys that can be pressed on the keyboard that our engine should be able to handle.
	namespace Key
	{
		/*
		* Mouse control.
		*/
		const unsigned int kMouseButtonLeft = VK_LBUTTON;
		const unsigned int kMouseButtonRight = VK_RBUTTON;
		const unsigned int kMouseButton3 = VK_MBUTTON;
		const unsigned int kMouseButton4 = VK_XBUTTON1;
		const unsigned int kMouseButton5 = VK_XBUTTON2;

		/*
		* Please use the order of a keyboard going left to right, then down row. Starting from the top.
		*/
		const unsigned int kEscape = VK_ESCAPE;

		const unsigned int kF1 = VK_F1;
		const unsigned int kF2 = VK_F2;
		const unsigned int kF3 = VK_F3;
		const unsigned int kF4 = VK_F4;

		const unsigned int kF5 = VK_F5;
		const unsigned int kF6 = VK_F6;
		const unsigned int kF7 = VK_F7;
		const unsigned int kF8 = VK_F8;

		const unsigned int kF9 = VK_F9;
		const unsigned int kF10 = VK_F10;
		const unsigned int kF11 = VK_F11;
		const unsigned int kF12 = VK_F12;

		const unsigned int k1 = 0x31;
		const unsigned int k2 = 0x32;
		const unsigned int k3 = 0x33;
		const unsigned int k4 = 0x34;
		const unsigned int k5 = 0x35;
		const unsigned int k6 = 0x36;
		const unsigned int k7 = 0x37;
		const unsigned int k8 = 0x38;
		const unsigned int k9 = 0x39;
		const unsigned int k0 = 0x30;

		const unsigned int kSubtract = VK_SUBTRACT;
		const unsigned int kAdd = VK_ADD;
		const unsigned int kBackspace = VK_BACK;
		
		const unsigned int kA = 0x41;
		const unsigned int kB = 0x42;
		const unsigned int kC = 0x43;
		const unsigned int kD = 0x44;
		const unsigned int kE = 0x45;
		const unsigned int kF = 0x46;
		const unsigned int kG = 0x47;
		const unsigned int kH = 0x48;
		const unsigned int kI = 0x49;
		const unsigned int kJ = 0x4A;
		const unsigned int kK = 0x4B;
		const unsigned int kL = 0x4C;
		const unsigned int kM = 0x4D;
		const unsigned int kN = 0x4E;
		const unsigned int kO = 0x4F;
		const unsigned int kP = 0x50;
		const unsigned int kQ = 0x51;
		const unsigned int kR = 0x52;
		const unsigned int kS = 0x53;
		const unsigned int kT = 0x54;
		const unsigned int kU = 0x55;
		const unsigned int kV = 0x56;
		const unsigned int kW = 0x57;
		const unsigned int kX = 0x58;
		const unsigned int kY = 0x59;
		const unsigned int kZ = 0x5A;

		const unsigned int kUp = VK_UP;
		const unsigned int kDown = VK_DOWN;
		const unsigned int kLeft = VK_LEFT;
		const unsigned int kRight = VK_RIGHT;
	}
}
#endif