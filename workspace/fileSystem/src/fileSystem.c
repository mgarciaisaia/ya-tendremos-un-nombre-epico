// Antes de incluir nada, para que no lo pisen
#define FUSE_USE_VERSION 26

#include <stdio.h>
#include <stdlib.h>
#include <fuse.h>

/**
 * Create and open a file
 *
 * If the file does not exist, first create it with the specified
 * mode, and then open it.
 *
 * If this method is not implemented or under Linux kernel
 * versions earlier than 2.6.15, the mknod() and open() methods
 * will be called instead.
 *
 * Introduced in version 2.5
 */
int fat32_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
	//FIXME: implementar
	return -1;
}

/** File open operation
 *
 * No creation, or truncation flags (O_CREAT, O_EXCL, O_TRUNC)
 * will be passed to open().  Open should check if the operation
 * is permitted for the given flags.  Optionally open may also
 * return an arbitrary filehandle in the fuse_file_info structure,
 * which will be passed to all file operations.
 *
 * Changed in version 2.2
 */
int fat32_open(const char *path, struct fuse_file_info *fi) {
	//FIXME: implementar
	return -1;
}

/** Read data from an open file
 *
 * Read should return exactly the number of bytes requested except
 * on EOF or error, otherwise the rest of the data will be
 * substituted with zeroes.  An exception to this is when the
 * 'direct_io' mount option is specified, in which case the return
 * value of the read system call will reflect the return value of
 * this operation.
 *
 * Changed in version 2.2
 */
// I don't fully understand the documentation above -- it doesn't
// match the documentation for the read() system call which says it
// can return with anything up to the amount of data requested. nor
// with the fusexmp code which returns the amount of data also
// returned by read.
int fat32_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	//FIXME: implementar
	return -1;
}

/** Write data to an open file
 *
 * Write should return exactly the number of bytes requested
 * except on error.  An exception to this is when the 'direct_io'
 * mount option is specified (see read operation).
 *
 * Changed in version 2.2
 */
// As  with read(), the documentation above is inconsistent with the
// documentation for the write() system call.
int fat32_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	//FIXME: implementar
	return -1;
}

/** Possibly flush cached data
 *
 * BIG NOTE: This is not equivalent to fsync().  It's not a
 * request to sync dirty data.
 *
 * Flush is called on each close() of a file descriptor.  So if a
 * filesystem wants to return write errors in close() and the file
 * has cached dirty data, this is a good place to write back data
 * and return any errors.  Since many applications ignore close()
 * errors this is not always useful.
 *
 * NOTE: The flush() method may be called more than once for each
 * open().  This happens if more than one file descriptor refers
 * to an opened file due to dup(), dup2() or fork() calls.  It is
 * not possible to determine if a flush is final, so each flush
 * should be treated equally.  Multiple write-flush sequences are
 * relatively rare, so this shouldn't be a problem.
 *
 * Filesystems shouldn't assume that flush will always be called
 * after some writes, or that if will be called at all.
 *
 * Changed in version 2.2
 */
int fat32_flush(const char *path, struct fuse_file_info *fi) {
	//FIXME: implementar
	return -1;
}

/** Release an open file
 *
 * Release is called when there are no more references to an open
 * file: all file descriptors are closed and all memory mappings
 * are unmapped.
 *
 * For every open() call there will be exactly one release() call
 * with the same flags and file descriptor.  It is possible to
 * have a file opened more than once, in which case only the last
 * release will mean, that no more reads/writes will happen on the
 * file.  The return value of release is ignored.
 *
 * Changed in version 2.2
 */
int fat32_release(const char *path, struct fuse_file_info *fi) {
	//FIXME: implementar
	return -1;
}

/** Change the size of a file */
int fat32_truncate(const char *path, off_t newsize) {
	//FIXME: implementar
	return -1;
}

/** Remove a file */
int fat32_unlink(const char *path) {
	//FIXME: implementar
	return -1;
}

/** Create a directory */
int fat32_mkdir(const char *path, mode_t mode) {
	//FIXME: implementar
	return -1;
}

/** Read directory
 *
 * This supersedes the old getdir() interface.  New applications
 * should use this.
 *
 * The filesystem may choose between two modes of operation:
 *
 * 1) The readdir implementation ignores the offset parameter, and
 * passes zero to the filler function's offset.  The filler
 * function will not return '1' (unless an error happens), so the
 * whole directory is read in a single readdir operation.  This
 * works just like the old getdir() method.
 *
 * 2) The readdir implementation keeps track of the offsets of the
 * directory entries.  It uses the offset parameter and always
 * passes non-zero offset to the filler function.  When the buffer
 * is full (or an error happens) the filler function will return
 * '1'.
 *
 * Introduced in version 2.3
 */
int fat32_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	//FIXME: implementar
	return -1;
}

/** Remove a directory */
int fat32_rmdir(const char *path) {
	//FIXME: implementar
	return -1;
}

/** Get file attributes.
 *
 * Similar to stat().  The 'st_dev' and 'st_blksize' fields are
 * ignored.  The 'st_ino' field is ignored except if the 'use_ino'
 * mount option is given.
 */
int fat32_getattr(const char *path, struct stat *statbuf) {
	//FIXME: implementar
	return -1;
}

/** Rename a file */
// both path and newpath are fs-relative
int fat32_rename(const char *path, const char *newpath) {
	//FIXME: implementar
	return -1;
}

static struct fuse_operations fat32_operations = {
		.create = fat32_create,
		.open = fat32_open,
		.read = fat32_read,
		.write = fat32_write,
		.flush = fat32_flush,
		.release = fat32_release,
		.truncate = fat32_truncate,
		.unlink = fat32_unlink,
		.mkdir = fat32_mkdir,
		.readdir = fat32_readdir,
		.rmdir = fat32_rmdir,
		.getattr = fat32_getattr,
		.rename = fat32_rename,
};

int main(int argc, char *argv[]) {
	return fuse_main(argc, argv, &fat32_operations, NULL);
}