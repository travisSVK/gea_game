#include "SpritesheetManager.hpp"
#include "RenderManager.hpp"
#include "nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

namespace engine
{
    namespace managers
    {
        std::unordered_map<unsigned int, std::vector<std::pair<SDL_Texture*, glm::ivec2>>> SpritesheetManager::ParseSpritesheet(const char* jsonPath, SDL_Renderer* renderer)
        {
            std::unordered_map<unsigned int, std::vector<std::pair<SDL_Texture*, glm::ivec2>>> spritesheetsMap;
            std::ifstream spritesheetDef(jsonPath);
            json j;
            spritesheetDef >> j;

            std::string folderPath = j["path"].get<std::string>();
            json spritesheets = j["spriteSheets"].get<json::array_t>();
            json sprites = j["sprites"].get<json::array_t>();


            for (auto& spritesheet : spritesheets)
            {
                json paths = spritesheet["paths"].get<json::array_t>();
                std::vector<std::pair<SDL_Texture*, glm::ivec2>> textures;
                for (auto& path : paths)
                {
                    std::string spritePath = folderPath;
                    spritePath += path["path"].get<std::string>();
                    int width = path["width"].get<int>();
                    int height = path["height"].get<int>();
                    SDL_Texture* texture = RenderManager::CreateTexture(renderer, spritePath.c_str());
                    textures.push_back(std::pair<SDL_Texture*, glm::ivec2>(texture, glm::ivec2(width, height)));
                }
                spritesheetsMap[spritesheet["typeId"].get<unsigned int>()] = textures;
            }

            for (auto& sprite : sprites)
            {
                std::vector<std::pair<SDL_Texture*, glm::ivec2>> textures;
                std::string spritePath = folderPath;
                spritePath += sprite["path"].get<std::string>();
                int width = sprite["width"].get<int>();
                int height = sprite["height"].get<int>();
                SDL_Texture* texture = RenderManager::CreateTexture(renderer, spritePath.c_str());
                textures.push_back(std::pair<SDL_Texture*, glm::ivec2>(texture, glm::ivec2(width, height)));
                spritesheetsMap[sprite["typeId"].get<unsigned int>()] = textures;
            }

            return spritesheetsMap;
        }

        std::unordered_map<unsigned int, std::pair<SDL_Texture*, glm::ivec2>> SpritesheetManager::ParseSprites(const char* jsonPath, SDL_Renderer* renderer)
        {
            std::unordered_map<unsigned int, std::pair<SDL_Texture*, glm::ivec2>> spritesMap;
            std::ifstream spritesheetDef(jsonPath);
            json j;
            spritesheetDef >> j;

            std::string folderPath = j["path"].get<std::string>();
            json sprites = j["sprites"].get<json::array_t>();
            for (auto& sprite : sprites)
            {
                std::string spritePath = folderPath;
                spritePath += sprite["path"].get<std::string>();
                int width = sprite["width"].get<int>();
                int height = sprite["height"].get<int>();
                SDL_Texture* texture = RenderManager::CreateTexture(renderer, spritePath.c_str());
                std::pair<SDL_Texture*, glm::ivec2> texturePair = std::pair<SDL_Texture*, glm::ivec2>(texture, glm::ivec2(width, height));
                spritesMap[sprite["typeId"].get<unsigned int>()] = texturePair;
            }

            return spritesMap;
        }
    }
}