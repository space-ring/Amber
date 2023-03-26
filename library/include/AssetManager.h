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

		struct ShaderFormula {
			list<token> V, TC, TE, G, F, C;
		};

		tokenMap<string> sourcePaths;
		tokenMap<string> meshPaths;
		tokenMap<string> texturePaths;

		tokenMap<string> sources;
		//todo other raw asset types
		tokenMap<RawMesh> rawMeshes;
		tokenMap<RawTexture> rawTextures;

		tokenMap<ShaderFormula> shaderFormulas;
		tokenMap<Shader> shaders;
		tokenMap<Mesh> meshes;
		tokenMap<Texture> textures;

	public:

		void addManifest(view path);

		// SHADERS //
		void addSourcePath(token id, view path);

		view loadSource(token id);

		void unloadSource(token id);

		view getSource(token id);

		void addShaderFormula(token id, const ShaderFormula& formula);

		Shader& loadShader(token id);

		void unloadShader(token id);

		Shader& getShader(token id);

		// MESHES //
		void addMeshPath(token id, view path);

		RawMesh& loadRawMesh(token id);

		void unloadRawMesh(token id);

		RawMesh& getRawMesh(token id);

		Mesh& getMesh(token id);

		// TEXTURES //
		void addTexturePath(token id, view path);

		RawTexture& loadRawTexture(token id);

		void unloadRawTexture(token id);

		RawTexture& getRawTexture(token id);

		Texture& getTexture(token id);
	};
}


#endif //ENGINE_ASSETMANAGER_H
