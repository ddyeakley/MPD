/*
 * Copyright (C) 2003-2015 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"
#include "Registry.hxx"
#include "StoragePlugin.hxx"
#include "plugins/LocalStorage.hxx"
#include "plugins/SmbclientStorage.hxx"
#include "plugins/NfsStorage.hxx"
#include "util/Error.hxx"

#include <assert.h>
#include <string.h>

const StoragePlugin *const storage_plugins[] = {
	&local_storage_plugin,
#ifdef ENABLE_SMBCLIENT
	&smbclient_storage_plugin,
#endif
#ifdef ENABLE_NFS
	&nfs_storage_plugin,
#endif
	nullptr
};

const StoragePlugin *
GetStoragePluginByName(const char *name)
{
	for (auto i = storage_plugins; *i != nullptr; ++i) {
		const StoragePlugin &plugin = **i;
		if (strcmp(plugin.name, name) == 0)
			return *i;
	}

	return nullptr;
}

Storage *
CreateStorageURI(EventLoop &event_loop, const char *uri, Error &error)
{
	assert(!error.IsDefined());

	for (auto i = storage_plugins; *i != nullptr; ++i) {
		const StoragePlugin &plugin = **i;

		if (plugin.create_uri == nullptr)
			continue;

		Storage *storage = plugin.create_uri(event_loop, uri, error);
		if (storage != nullptr || error.IsDefined())
			return storage;
	}

	return nullptr;
}
