// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Core module"
// For conditions of distribution and use, see copyright notice in Export.hpp

#include <Nazara/Core/UniformBuffer.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Core/ErrorFlags.hpp>
#include <Nazara/Core/Export.hpp>

namespace Nz
{
	UniformBuffer::UniformBuffer(std::shared_ptr<Buffer> buffer) :
	m_buffer(std::move(buffer)),
	m_startOffset(0)
	{
		NazaraAssertMsg(m_buffer, "invalid buffer");
		NazaraAssertMsg(m_buffer->GetType() == BufferType::Uniform, "buffer must be an uniform buffer");

		m_endOffset = m_buffer->GetSize();
	}

	UniformBuffer::UniformBuffer(std::shared_ptr<Buffer> buffer, UInt64 offset, UInt64 size) :
	m_buffer(std::move(buffer)),
	m_endOffset(size),
	m_startOffset(offset)
	{
	}

	UniformBuffer::UniformBuffer(UInt64 size, BufferUsageFlags usage, const BufferFactory& bufferFactory, const void* initialData) :
	m_endOffset(size),
	m_startOffset(0)
	{
		NazaraAssertMsg(size > 0, "invalid size");

		m_buffer = bufferFactory(BufferType::Uniform, size, usage, initialData);
	}

	bool UniformBuffer::Fill(const void* data, UInt64 offset, UInt64 size)
	{
		NazaraAssertMsg(m_buffer, "Invalid buffer");
		NazaraAssertMsg(m_startOffset + offset + size <= m_endOffset, "Exceeding virtual buffer size");

		return m_buffer->Fill(data, m_startOffset + offset, size);
	}

	void* UniformBuffer::Map(UInt64 offset, UInt64 size)
	{
		NazaraAssertMsg(m_buffer, "Invalid buffer");
		NazaraAssertMsg(m_startOffset + offset + size <= m_endOffset, "Exceeding virtual buffer size");

		return m_buffer->Map(m_startOffset + offset, size);
	}

	void* UniformBuffer::Map(UInt64 offset, UInt64 size) const
	{
		NazaraAssertMsg(m_buffer, "Invalid buffer");
		NazaraAssertMsg(m_startOffset + offset + size <= m_endOffset, "Exceeding virtual buffer size");

		return m_buffer->Map(m_startOffset + offset, size);
	}

	void UniformBuffer::Unmap() const
	{
		m_buffer->Unmap();
	}
}
