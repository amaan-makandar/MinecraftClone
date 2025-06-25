#include "ChunkManager.h"

/*@TODO:
	- fix chunk rendering around camera position
*/

ChunkManager::ChunkManager(Camera* camera, GLFWwindow* window)
	: camera(camera), window(window)
{
}

static long maxFloats = 0;
static long maxRendered = 0;

void ChunkManager::Render_Chunks()
{
	int tempRendered = 0;

	

	for (auto& iter: loadedChunks) {
		iter.second->render();

		//test code to find the max number of floats used in mesh generation
		if (iter.second->numFloats() > maxFloats)
			maxFloats = iter.second->numFloats();
		tempRendered++;


	}

	if (tempRendered > maxRendered)
		maxRendered = tempRendered;
}

void ChunkManager::printMaxFloats() {
	printf("Max floats: %ld \n", maxFloats);
	printf("Max chunks rendered: %d", maxRendered);
}

//clear
void ChunkManager::Clear_Loaded_Chunks() {
	loadedChunks.clear();
}

/*
	- update load list
	- update unload list
	- add extra chunks if necessary?
*/
void ChunkManager::Update_Loaded_Chunks()
{
	//clear loaded chunks before computation
	Clear_Loaded_Chunks();

	//need to create new chunks around player and use ones already in chunkList
	glm::vec3 camPos = camera->getPosition();

	//loop through square centered at player position and only render chunks in circle around camera
	//@TODO: 
	//	- only update the loaded chunks list when a chunk border is crossed
	//	- change rendering to center at cameraPos
	for (int i = static_cast<int>(camPos.x) - radius; i < static_cast<int>(camPos.x) + radius; i++) {
		for (int j = static_cast<int>(camPos.z) - radius; j < static_cast<int>(camPos.z) + radius; j++) {
			//chunks only generate at integer multiples of the chunk size
			if (i % CHUNK_SIZE == 0 && j % CHUNK_SIZE == 0) {

				if (chunkList.find({ i, j }) != chunkList.end()) {
					loadedChunks.insert({ glm::ivec2(i, j) , chunkList.find({i, j})->second });
				}
				else 
				{
					//inserts new chunk into each chunk list and sets its data
					chunkList.insert({ glm::ivec2{i , j}, new Chunk(camera, &chunkData, i, j)});
					loadedChunks.insert({ {glm::ivec2{i, j}, chunkList.find(glm::ivec2{i, j})->second} });
					loadedChunks.find(glm::ivec2{ i,j })->second->setData();
				}
			}
		}
	}
}

