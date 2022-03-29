#pragma once

#include <memory>
#include <vector>

#include "resources/mesh.hpp"
#include "resources/texture.hpp"
#include "resources/program_shader.hpp"
#include "game/transform.hpp"

namespace Renderer
{
	class Model
	{
	public:
		// Every meshes that this model posseses.
		std::vector<std::shared_ptr<Resources::Mesh>> meshes;

		Game::Transform transform;

		Model() = default;

		/**
		 * Create a model that contains meshes with embedded textures using Assimp.
		 *
		 * @param i_meshFilename
		 */
		Model(const std::string& i_meshFilename);

		/**
		 * Create a model manually specifying mesh and texture.
		 *
		 * @param i_meshFilename
		 * @param i_textureFilename
		 * @param i_flipTexture
		 */
		Model(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture);

		/**
		 * Add a cube shaped mesh after the creation of this model object.
		 *
		 */
		void AddCubeMesh();

		/**
		 * Add a sphere shaped mesh after the creation of this model object.
		 *
		 * @param i_radius
		 * @param i_longitude
		 * @param i_latitude
		 */
		void AddSphereMesh(const float i_radius = 1.f, const int i_longitude = 50, const int i_latitude = 25);

		/**
		* Add a mesh coming from a 3D model file after the creation of this model object.
		* The specified mesh may contain embedded texture.
		* 
		*/
		void AddMeshesFromFile(const std::string& i_meshFilename);

		/**
		* Add a mesh coming from a 3D model file, manually specifying its texture, after the creation of this model object.
		* 
		*/
		void AddMeshesFromFile(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture);

		/**
		 * Add a texture after the creation of this model object.
		 *
		 * @param i_textureFilename
		 * @param i_flipTexture
		 * @param i_meshIndex
		 */
		void AddTextureToMesh(const std::string& i_textureFilename, const bool i_flipTexture, const unsigned int i_meshIndex);
	};
};
