//
// Created by croissant on 06/06/2022.
//

#ifndef ENGINE_ASSETMANAGER_H
#define ENGINE_ASSETMANAGER_H

#include <map>
#include <string>
#include <list>
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

namespace Amber {

	class AssetManager {
		using token = unsigned long long int;
		template<class T> using tokenMap = std::map<token, T>;
		template<class T> using list = std::list<T>;
		using view = std::string_view;
		using string = std::string;

		tokenMap<string> shaderPaths;
		tokenMap<string> meshPaths;
		tokenMap<string> texturePaths;

		tokenMap<string> rawShaders;
		//todo other raw asset types
		tokenMap<RawMesh> rawMeshes;
		tokenMap<RawTexture> rawTextures;

		tokenMap<Shader> shaders;
		tokenMap<Mesh> meshes;
		tokenMap<Texture> textures;

	public:

		AssetManager();

		~AssetManager();

		void addManifest(view path);

		void addShaderPath(token id, view path);

		void addMeshPath(token id, view path);

		void addTexturePath(token id, view path);

		view getRawShader(token id, bool load = true);

		RawMesh& getRawMesh(token id, bool load = true);

		RawTexture& getRawTexture(token id, bool load = true);

		//note maybe remove loading functions
		view loadRawShader(token id);

		RawMesh& loadRawMesh(token id);

		RawTexture& loadRawTexture(token id);

		void unloadRawShader(token id);

		void unloadRawMesh(token id);

		void unloadRawTexture(token id);

		Shader& createShader(token id,
		                     const list<token>& vertex,
		                     const list<token>& tessControl,
		                     const list<token>& tessEval,
		                     const list<token>& geometry,
		                     const list<token>& fragment,
		                     const list<token>& compute);
//todo concatenate? manifest list [vid1,vid2,vid3],[f1,f2]


		Shader& getShader(token id);

		Mesh& getMesh(token id);

		Texture& getTexture(token id);
	};
}


#endif //ENGINE_ASSETMANAGER_H
