//
// Created by croissant on 06/06/2022.
//

#ifndef ENGINE_ASSETMANAGER_H
#define ENGINE_ASSETMANAGER_H

#include <map>
#include <string>
#include "Mesh.h"
#include "Texture.h"
#include "engineIO.h"
#include "Singleton.h"

namespace Amber {

	class AssetManager : public Singleton<AssetManager> {
		friend Singleton<AssetManager>;
		using string = std::string;
		template<class T> using stringMap = std::map<string, T>;

	private: //todo const maps?
		stringMap<compoundShader>* shaderPaths = new stringMap<compoundShader>;
		stringMap<string>* meshPaths = new stringMap<string>;
		stringMap<string>* texturePaths = new stringMap<string>;
		stringMap<Shader*>* shaders = new stringMap<Shader*>;
		stringMap<Mesh*>* meshes = new stringMap<Mesh*>;
		stringMap<Texture*>* textures = new stringMap<Texture*>;

		AssetManager();

		virtual ~AssetManager();

	public:

		void buildAll();

		void addManifest(const string& manifest);

		void addShader(const string& name, const compoundShader& paths);

		void addShader(const string& name, Shader& shader);

		void addMesh(const string& name, const string& path);

		void addMesh(const string& name, Mesh& mesh);

		void addTexture(const string& name, const string& path);

		void addTexture(const string& name, Texture& texture);

		Shader* getShader(const string& name);

		Mesh* getMesh(const string& name);

		Texture* getTexture(const string& name);
	};
}


#endif //ENGINE_ASSETMANAGER_H
