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

#ifndef MPD_FILE_OUTPUT_STREAM_HXX
#define MPD_FILE_OUTPUT_STREAM_HXX

#include "check.h"
#include "OutputStream.hxx"
#include "fs/AllocatedPath.hxx"
#include "Compiler.h"

#ifndef WIN32
#include "system/FileDescriptor.hxx"
#endif

#include <assert.h>

#ifdef WIN32
#include <windows.h>
#endif

class Path;

class FileOutputStream final : public OutputStream {
	AllocatedPath path;

#ifdef WIN32
	HANDLE handle;
#else
	FileDescriptor fd;
#endif

#ifdef HAVE_LINKAT
	/**
	 * Was O_TMPFILE used?  If yes, then linkat() must be used to
	 * create a link to this file.
	 */
	bool is_tmpfile;
#endif

public:
	FileOutputStream(Path _path, Error &error);

	~FileOutputStream() {
		if (IsDefined())
			Cancel();
	}

	static FileOutputStream *Create(Path path, Error &error);

	bool IsDefined() const {
#ifdef WIN32
		return handle != INVALID_HANDLE_VALUE;
#else
		return fd.IsDefined();
#endif
	}

	bool Commit(Error &error);
	void Cancel();

	/* virtual methods from class OutputStream */
	bool Write(const void *data, size_t size, Error &error) override;
};

#endif
