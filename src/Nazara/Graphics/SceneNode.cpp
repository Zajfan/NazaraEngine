// Copyright (C) 2014 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Graphics/SceneNode.hpp>
#include <Nazara/Graphics/Config.hpp>
#include <Nazara/Graphics/Scene.hpp>
#include <Nazara/Graphics/Debug.hpp>

///FIXME: Constructeur de copie

NzSceneNode::NzSceneNode() :
m_scene(nullptr),
m_drawingEnabled(true),
m_visible(false)
{
}

NzSceneNode::NzSceneNode(const NzSceneNode& sceneNode) :
NzNode(sceneNode),
m_scene(nullptr),
m_drawingEnabled(sceneNode.m_drawingEnabled),
m_visible(false)
{
}

NzSceneNode::~NzSceneNode() = default;

void NzSceneNode::EnableDrawing(bool drawingEnabled)
{
	m_drawingEnabled = drawingEnabled;
}

NzVector3f NzSceneNode::GetBackward() const
{
	if (m_scene)
	{
		if (!m_derivedUpdated)
			UpdateDerived();

		return m_derivedRotation * m_scene->GetBackward();
	}
	else
		return NzNode::GetBackward();
}

NzVector3f NzSceneNode::GetDown() const
{
	if (m_scene)
	{
		if (!m_derivedUpdated)
			UpdateDerived();

		return m_derivedRotation * m_scene->GetDown();
	}
	else
		return NzNode::GetDown();
}

NzVector3f NzSceneNode::GetForward() const
{
	if (m_scene)
	{
		if (!m_derivedUpdated)
			UpdateDerived();

		return m_derivedRotation * m_scene->GetForward();
	}
	else
		return NzNode::GetForward();
}

NzVector3f NzSceneNode::GetLeft() const
{
	if (m_scene)
	{
		if (!m_derivedUpdated)
			UpdateDerived();

		return m_derivedRotation * m_scene->GetLeft();
	}
	else
		return NzNode::GetLeft();
}

nzNodeType NzSceneNode::GetNodeType() const
{
	return nzNodeType_Scene;
}

NzVector3f NzSceneNode::GetRight() const
{
	if (m_scene)
	{
		if (!m_derivedUpdated)
			UpdateDerived();

		return m_derivedRotation * m_scene->GetRight();
	}
	else
		return NzNode::GetRight();
}

NzScene* NzSceneNode::GetScene() const
{
	return m_scene;
}

NzVector3f NzSceneNode::GetUp() const
{
	if (m_scene)
	{
		if (!m_derivedUpdated)
			UpdateDerived();

		return m_derivedRotation * m_scene->GetUp();
	}
	else
		return NzNode::GetUp();
}

bool NzSceneNode::IsDrawingEnabled() const
{
	return m_drawingEnabled;
}

bool NzSceneNode::IsVisible() const
{
	return m_visible;
}

NzSceneNode& NzSceneNode::operator=(const NzSceneNode& sceneNode)
{
	NzNode::operator=(sceneNode);

	// La scène est affectée via le parenting du node
	m_drawingEnabled = sceneNode.m_drawingEnabled;
	m_visible = false;

	return *this;
}

NzSceneNode& NzSceneNode::operator=(NzSceneNode&& sceneNode)
{
	NzNode::operator=(sceneNode);

	// La scène est affectée via le parenting du node
	m_drawingEnabled = sceneNode.m_drawingEnabled;
	m_visible = sceneNode.m_visible;

	return *this;
}

bool NzSceneNode::FrustumCull(const NzFrustumf& frustum) const
{
	return frustum.Contains(GetBoundingVolume());
}

void NzSceneNode::OnParenting(const NzNode* parent)
{
	if (parent)
	{
		///FIXME: Remonter jusqu'au premier parent de type SceneNode plutôt que de s'arrêter au premier venu
		if (parent->GetNodeType() == nzNodeType_Scene)
			SetScene(static_cast<const NzSceneNode*>(parent)->m_scene);
	}
	else
		SetScene(nullptr);
}

void NzSceneNode::OnVisibilityChange(bool visibility)
{
	NazaraUnused(visibility);

	///TODO: Envoyer l'évènements aux listeners
}

void NzSceneNode::RecursiveSetScene(NzScene* scene, NzNode* node)
{
	const std::vector<NzNode*>& childs = node->GetChilds();
	for (NzNode* child : childs)
	{
		if (child->GetNodeType() == nzNodeType_Scene)
		{
			NzSceneNode* sceneNode = static_cast<NzSceneNode*>(child);
			sceneNode->SetScene(scene);
		}

		if (node->HasChilds())
			RecursiveSetScene(scene, node);
	}
}

void NzSceneNode::Register()
{
}

void NzSceneNode::SetScene(NzScene* scene)
{
	if (m_scene != scene)
	{
		if (m_scene)
			Unregister();

		m_scene = scene;
		if (m_scene)
			Register();

		RecursiveSetScene(scene, this);
	}
}

void NzSceneNode::Unregister()
{
}

void NzSceneNode::Update()
{
}

void NzSceneNode::UpdateVisibility(const NzFrustumf& frustum)
{
	bool wasVisible = m_visible;

	if (m_drawingEnabled)
	{
		#if NAZARA_GRAPHICS_SAFE
		if (!IsDrawable())
		{
			NazaraError("SceneNode is not drawable");
			return;
		}
		#endif

		m_visible = FrustumCull(frustum);
	}
	else
		m_visible = false;

	if (m_visible != wasVisible)
		OnVisibilityChange(m_visible);
}
