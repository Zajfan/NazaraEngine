// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Vulkan renderer"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/VulkanRenderer/Wrapper/Pipeline.hpp>
#include <Nazara/VulkanRenderer/Utils.hpp>
#include <Nazara/Utils/StackArray.hpp>
#include <Nazara/VulkanRenderer/Debug.hpp>

namespace Nz
{
	namespace Vk
	{
		inline Pipeline::Pipeline() :
		m_handle(VK_NULL_HANDLE)
		{
		}

		inline Pipeline::Pipeline(Pipeline&& object) noexcept :
		m_device(std::move(object.m_device)),
		m_allocator(object.m_allocator),
		m_handle(object.m_handle),
		m_lastErrorCode(object.m_lastErrorCode)
		{
			object.m_handle = VK_NULL_HANDLE;
		}

		inline Pipeline::~Pipeline()
		{
			Destroy();
		}

		inline bool Pipeline::CreateCompute(Device& device, const VkComputePipelineCreateInfo& createInfo, VkPipelineCache cache, const VkAllocationCallbacks* allocator)
		{
			return Create(device, device.vkCreateComputePipelines(device, cache, 1U, &createInfo, allocator, &m_handle), allocator);
		}

		inline bool Pipeline::CreateGraphics(Device& device, const VkGraphicsPipelineCreateInfo& createInfo, VkPipelineCache cache, const VkAllocationCallbacks* allocator)
		{
			return Create(device, device.vkCreateGraphicsPipelines(device, cache, 1U, &createInfo, allocator, &m_handle), allocator);
		}

		inline void Pipeline::Destroy()
		{
			if (m_handle != VK_NULL_HANDLE)
			{
				m_device->vkDestroyPipeline(*m_device, m_handle, (m_allocator.pfnAllocation) ? &m_allocator : nullptr);
				m_handle = VK_NULL_HANDLE;
			}
		}

		inline Device& Pipeline::GetDevice() const
		{
			return *m_device;
		}

		inline VkResult Pipeline::GetLastErrorCode() const
		{
			return m_lastErrorCode;
		}

		inline void Pipeline::SetDebugName(std::string_view name)
		{
			return m_device->SetDebugName(VK_OBJECT_TYPE_PIPELINE, static_cast<UInt64>(reinterpret_cast<std::uintptr_t>(m_handle)), name);
		}

		inline Pipeline::operator VkPipeline() const
		{
			return m_handle;
		}

		inline bool Pipeline::Create(Device& device, VkResult result, const VkAllocationCallbacks* allocator)
		{
			m_device = &device;
			m_lastErrorCode = result;
			if (m_lastErrorCode != VkResult::VK_SUCCESS)
			{
				NazaraError("Failed to create Vulkan object: " + TranslateVulkanError(m_lastErrorCode));
				return false;
			}

			// Store the allocator to access them when needed
			if (allocator)
				m_allocator = *allocator;
			else
				m_allocator.pfnAllocation = nullptr;

			return true;
		}
	}
}

#include <Nazara/VulkanRenderer/DebugOff.hpp>
