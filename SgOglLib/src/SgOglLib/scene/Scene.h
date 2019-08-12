#pragma once

#include "Node.h"
#include "Core.h"

namespace sg::ogl::scene
{
	class SG_OGL_API Scene : public Node
	{
	public:
		//-------------------------------------------------
		// Ctors. / Dtor.
		//-------------------------------------------------

		Scene();

		Scene(const Scene& t_other) = delete;
		Scene(Scene&& t_other) noexcept = delete;
		Scene& operator=(const Scene& t_other) = delete;
		Scene& operator=(Scene&& t_other) noexcept = delete;

		~Scene() noexcept;

		//-------------------------------------------------
		// Scene objects
		//-------------------------------------------------

		void AddObject(Node* t_node) const;

	protected:

	private:
		Node* m_rootNode{ nullptr };
	};
}
