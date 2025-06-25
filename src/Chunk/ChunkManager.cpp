#include "ChunkManager.h"

int round_up(int num_to_round, int mult);

ChunkManager::ChunkManager(Camera* camera, GLFWwindow* window)
	: camera(camera), window(window)
{
}

void ChunkManager::Render_Chunks()
{
	for (auto& iter: loadedChunks) {
		iter.second->render();
	}
}

void ChunkManager::Clear_Loaded_Chunks() {
	loadedChunks.clear();
}


void ChunkManager::Update_Loaded_Chunks()
{
	Clear_Loaded_Chunks();

	glm::vec3 cam_pos = camera->getPosition();

	int iz = -radius;

	while (iz < radius) {
		int dx = (int)sqrt(radius * radius - iz * iz);
		int j = round_up(static_cast<int>(cam_pos.z) + iz, CHUNK_SIZE);
		int ix = -dx;
		
		while (ix < dx) {
			// chunk at cam_pos.x + ix, cam_pos.z + iz; round up to chunk size
			int i = round_up(static_cast<int>(cam_pos.x) + ix, CHUNK_SIZE);
			
			auto possible_chunk = chunkList.find({i, j});
			if (possible_chunk != chunkList.end()) {
				loadedChunks.insert({{i, j}, possible_chunk->second});
			}
			else {
				Chunk* new_chunk = new Chunk(camera, &chunkData, i, j);
				chunkList.insert({ glm::ivec2{i , j}, new_chunk });
				loadedChunks.insert({ {glm::ivec2{i, j}, new_chunk} });
				new_chunk->setData();
			}

			ix += CHUNK_SIZE;
		}

		iz += CHUNK_SIZE;
	}
}

int round_up(int num_to_round, int mult) {
	if (mult == 0)
		return num_to_round;

	int rem = abs(num_to_round) % mult;
	if (rem == 0)
		return num_to_round;

	if (num_to_round < 0)
		return -(abs(num_to_round) - rem);
	else
		return num_to_round + mult - rem;
}