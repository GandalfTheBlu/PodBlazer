#include "point_system.h"
#include <math.h>

PointSystem::PointSystem() {
}
void PointSystem::PassCheckpoint(std::vector<glm::vec2> mapData,glm::vec3 playerPos)
{
    printf("player pos x is %f\n", playerPos.x);
    printf("player pos y is %f\n", playerPos.z);
    printf("checkpoint x is %f\n", mapData[currentCheckPoint].x);
    printf("checkpont y is %f\n", mapData[currentCheckPoint].y);
    float distanceToCheckpoint = sqrt((mapData[currentCheckPoint].x-playerPos.x)*(mapData[currentCheckPoint].x - playerPos.x)+ (mapData[currentCheckPoint].y - playerPos.z) * (mapData[currentCheckPoint].y - playerPos.z));
    printf("distance is: %f\n", distanceToCheckpoint);
        if(distanceToCheckpoint <=6) {
            currentCheckPoint++;
            currentPoints++;
            if (currentCheckPoint >= checkpointAmount) {
                currentCheckPoint = 0;
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

	currentCheckPoint = closestPoint;
	checkpointAmount = mapData.size();
}
