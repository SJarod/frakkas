#pragma once

#include <memory>

#include "renderer/render_object.hpp"
#include "animation/animator.hpp"

#include "game/transform.hpp"

namespace Resources
{
	struct Bone;
	class SkeletalMesh;

	class Serializer;
}

namespace Renderer
{
	class SkeletalModel : public Renderer::RenderObject
	{
	public:
		std::shared_ptr<Resources::Shader> lightDepthShader;

		std::shared_ptr<Resources::SkeletalMesh>	skmesh;
		Animation::Animator							player;
		std::shared_ptr<SkeletalAnimationPack>		skpack;

		SkeletalModel();

		/**
		 * @Summary Create a model that contains submeshes with embedded textures using Assimp.
		 * @param i_meshFilename
		 */
		SkeletalModel(const std::string& i_meshFilename);

		/**
		 * @Summary Create a model manually specifying mesh and texture.
		 *
		 * @param i_meshFilename
		 * @param i_textureFilename
		 * @param i_flipTexture
		 */
		SkeletalModel(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture);

		/**
		* @Summary Set a mesh coming from a 3D model file after the creation of this model object.
		* The specified mesh may contain embedded texture.
		* User can set "ProceduralCube" or "ProceduralSphere" as filename to procedurally load a cube or a sphere.
		*/
		void SetSkeletalMeshFromFile(const std::string& i_meshFilename);

		/**
		* @Summary Set a mesh coming from a 3D model file, manually specifying its texture, after the creation of this model object.
		* User can set "ProceduralCube" or "ProceduralSphere" as filename to procedurally load a cube or a sphere.
		* If user uses procedural loading, the specified texture will not be precessed.
		*/
		void SetSkeletalMeshFromFile(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture);

		/**
		 * @Summary Set a texture after the creation of this model object.
		 *
		 * @param i_textureFilename
		 * @param i_flipTexture
		 * @param i_submeshIndex
		 */
		void SetTextureToSubmesh(const std::string& i_textureFilename, const bool i_flipTexture, const unsigned int i_submeshIndex) const;

		/**
		 * @brief Set one texture to every submesh of the model's mesh.
		 * @param i_textureFilename The path from Engine root to the texture
		 * @param i_flipTexture Is the texture flip horizontally or not:
		 */
		void SetTextureToAllSubmesh(const std::string& i_textureFilename, const bool i_flipTexture) const;

		/**
		 * @Summary Load every animations from a file into the SkeletalAnimationPack.
		 * The animations are loaded based on the specific SkeletalMesh of this SkeletalModel.
		 * 
		 * @param i_animationFilename
		 */
		void LoadAnimationsForThis(const std::string& i_animationFilename);

		/**
		 * @Summary Setup entity components from input file.
		 * @param i_file the opened input file.
		 */
		void Read(std::ifstream& i_file, const Resources::Serializer& i_serializer) {};

		/**
		 * @Summary Write the entity components in scene text format.
		 * @param o_file the opened output file.
		 */
		void Write(std::ofstream& o_file, const Resources::Serializer& i_serializer) const {};
	};
};
