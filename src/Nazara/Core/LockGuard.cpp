// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Core module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Core/LockGuard.hpp>
#include <Nazara/Core/Mutex.hpp>
#include <Nazara/Core/Debug.hpp>

namespace Nz
{
	LockGuard::LockGuard(Mutex& mutex) :
	m_mutex(mutex)
	{
		m_mutex.Lock();
	}

	LockGuard::~LockGuard()
	{
		m_mutex.Unlock();
	}
}
