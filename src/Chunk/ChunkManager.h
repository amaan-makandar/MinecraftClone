#pragma once

#include <unordered_map>
#include "Chunk.h"
#include "glm/glm.hpp"
#include <math.h>
#include "glm/gtx/hash.hpp"
#include "MousePicker.h"
#include "GLFW/glfw3.h"
#include "StaticChunkData.h"


class ChunkManager {
private:
	//list of all chunks
	std::unordered_map<glm::ivec2, Chunk*> chunkList;
	//list of loaded chunks
	std::unordered_map<glm::ivec2, Chunk*> loadedChunks;

	StaticChunkData chunkData;

	Camera* camera;
	GLFWwindow* window;

	const static int radius = 16 * CHUNK_SIZE;

public:

	ChunkManager(Camera *camera, GLFWwindow* window);

	//render all chunks in visibility list
	void Render_Chunks();

	void Clear_Loaded_Chunks();

	//update loaded chunk list to contain only chunks within some distance
	//of the camera position
	void Update_Loaded_Chunks();

	//test function to print the max number of floats used by a single chunk
	void printMaxFloats();
};