// Copyright (C) 2015 Alexandre Janniaux
// This file is part of the "Nazara Engine - Core module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Core/Posix/MutexImpl.hpp>
#include <Nazara/Core/Debug.hpp>

namespace Nz
{
	MutexImpl::MutexImpl()
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

		pthread_mutex_init(&m_handle, &attr);
	}

	MutexImpl::~MutexImpl()
	{
		pthread_mutex_destroy(&m_handle);
	}

	void MutexImpl::Lock()
	{
		pthread_mutex_lock(&m_handle);
	}

	bool MutexImpl::TryLock()
	{
		return pthread_mutex_trylock(&m_handle) == 0;
	}

	void MutexImpl::Unlock()
	{
		pthread_mutex_unlock(&m_handle);
	}
}
