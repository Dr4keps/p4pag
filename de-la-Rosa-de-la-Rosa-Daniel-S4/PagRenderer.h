#pragma once


class PagRenderer
{
public:
	~PagRenderer();

	static PagRenderer* getInstance();

	void refreshCallback();
	void framebufferSizeCallback(int width, int height);
	void keyCallback(int key, int scancode, int action, int mods);
	void mouseButtonCallback(int button, int action, int mods);
	void scrollCallback(double xoffset, double yoffset);
	void prepareOpenGL();

private:
	PagRenderer();

	static PagRenderer *instance;
};

