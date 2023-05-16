#include "point_system.h"
#include <math.h>
#include "file_manager.h"
#include <utility>

PointSystem::PointSystem() {
}
void PointSystem::PassCheckpoint(std::vector<glm::vec2> mapData,glm::vec3 playerPos)
{
    float distanceToCheckpoint = sqrt((mapData[currentCheckPoint].x-playerPos.x)*(mapData[currentCheckPoint].x - playerPos.x)+ (mapData[currentCheckPoint].y - playerPos.z) * (mapData[currentCheckPoint].y - playerPos.z));
    
    if(distanceToCheckpoint <=6) {
        currentCheckPoint++;
        currentPoints++;
        if (currentCheckPoint >= checkpointAmount) {
            currentCheckPoint = 0;
        }
        if (currentPoints > highScore) {
            highScore = currentPoints;
        }
        if (currentCheckPoint == startCheckpoint)
        {
            lapsCompleted++;
        }
    }
}

void PointSystem::SetStartPoint(std::vector<glm::vec2> mapData, glm::vec3 playerPos)
{
	int closestPoint = 0;
	float currentClosestDistance = 999999;
	for (int i = 0; i < mapData.size(); i++)
	{
		float distanceToCheckpoint = sqrt((mapData[i].x - playerPos.x) * (mapData[i].x - playerPos.x) + (mapData[i].y - playerPos.z) * (mapData[i].y - playerPos.z));
		if (distanceToCheckpoint < currentClosestDistance) {
			currentClosestDistance = distanceToCheckpoint;
			closestPoint = i;
		}
	}

    for (int i = 0; i < mapData.size() - 1; i++) {
        float distanceBetweenPoints = sqrt((mapData[i].x - mapData[i+1].x) * (mapData[i].x - mapData[i + 1].x) + (mapData[i].y - mapData[i + 1].y) * (mapData[i].y - mapData[i + 1].y));
        distances.push_back(distanceBetweenPoints);
    }

	currentCheckPoint = closestPoint;
    startCheckpoint = closestPoint;
	checkpointAmount = mapData.size();
}

void PointSystem::LoadHighScore() {
    std::string text;
    Engine::ReadTextFile("scores/score.txt", text);
    highScore= std::stoi(text);
}

void PointSystem::SaveHighScore() {
    Engine::WriteTextFile("scores/score.txt", std::to_string(highScore),false);
}

void PointSystem::ResetScore() {
    currentPoints = 0;
    currentCheckPoint = startCheckpoint;
    lapsCompleted = 0;
}