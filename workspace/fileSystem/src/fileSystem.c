// Antes de incluir nada, para que no lo pisen
#define FUSE_USE_VERSION 26

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <fuse.h>
#include "src/utils.h"
#include "src/almacenamiento.h"

struct fat32_bootRecord {
	char jumpInstruction[3];
	char oemName[8];
	int16_t sectorSize;
	char clusterSize;
	int16_t fatOffset; // Sectores desde el inicio de este sector hasta la primer FAT (mínimo 1: el boot sector)
	char fatsCount; // Cantidad de FATs
	int16_t uselessField1;
	int16_t sectorCountSmall; // Cantidad de sectores del disco, o 0 si son mas de 64k
	char deviceType;
	int16_t uselessField2;
	int16_t trackSectors; // Sectores por pista (S de CHS)
	int16_t heads; // H de CHS
	int32_t bootRecordOffset;
	int32_t sectorCountLarge; // Cantidad de sectores del disco si son mas de 64k, ver sectorCountSmall sino
	int32_t fatSize; // Sectores por FAT
	int16_t fatConversionFlags; // Flags de la conversion FAT12/16 => FAT32
	int16_t fatVersion; // 0 = FAT32
	int32_t rootDirectory;
	int16_t fileSystemInfoSector;
	int16_t bootSectorBackup;
	char uselessField3[12];
	char physicalDeviceType; // 0x00 = removible; 0x80 = disco rigido
	char uselessField4;
	char extendedBootSignature;
	int32_t serialNumber;
	char volumeLabel[11];
	char fatType[8]; // Siempre "FAT32   "
	char bootCode[420]; // "NO LE INTERESA A NADIE!!" :)
	char bootSectorSignature[2]; // Siempre 0x55 0xAA
}__attribute__ ((__packed__));

struct fat32_config_boot_sector {
	int16_t sectorSize;
	char clusterSize;
	int16_t fatOffset; // Sectores desde el inicio de este sector hasta la primer FAT (mínimo 1: el boot sector)
	int32_t bootRecordOffset;
	int32_t sectorCount; // Cantidad de sectores del disco si son mas de 64k, ver sectorCountSmall sino
	int32_t fatSize; // Sectores por FAT
	int16_t fatVersion; // 0 = FAT32
	int32_t rootDirectory;
	int16_t fileSystemInfoSector;
	int16_t bootSectorBackup;
}__attribute__ ((__packed__));

struct fileSystemInfoSector {
	int32_t signature; //FS information sector signature (0x52 0x52 0x61 0x41 / "RRaA")
	char reserved[480]; //Reserved (byte values are 0x00)
	int32_t signature_2; //FS information sector signature (0x72 0x72 0x41 0x61 / "rrAa")
	int32_t free_clusters; //Number of free clusters on the drive, or -1 if unknown
	int32_t recently_cluster; //Number of the most recently allocated cluster
	char reserved_2[12]; //Reserved (byte values are 0x00)
	int32_t signature_3; // FS information sector signature (0x00 0x00 0x55 0xAA)
}__attribute__ ((__packed__));

void leerBootSector(fat32_sector bootSector,
		struct fat32_config_boot_sector * configBootSector) {
	configBootSector->clusterSize = bootSector[0x0D];
	configBootSector->bootRecordOffset = swap_int32( (int32_t) bootSector[0x1C] );
	configBootSector->sectorSize = swap_int16((int16_t) bootSector[0x0B]);
	configBootSector->fatOffset = swap_int16((int16_t) bootSector[0x0E]);
	configBootSector->sectorCount= swap_int32((int32_t) bootSector[0x20]);
	configBootSector->fatSize=swap_int32((int32_t) bootSector[0x24]);
	configBootSector->fatVersion = swap_int16((int16_t) bootSector[0x2A]);
	configBootSector->rootDirectory= swap_int32((int32_t) bootSector[0x2C]);
	configBootSector->fileSystemInfoSector= swap_int16((int16_t) bootSector[0x30]);
	configBootSector->bootSectorBackup= swap_int16((int16_t) bootSector[0x32]);
}

void setFSInfoSector(struct fileSystemInfoSector *fsInfoSector){
	fsInfoSector->free_clusters= swap_int32(-1);
}

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
int fat32_create(const char *path, mode_t mode, struct fuse_file_info *fileInfo) {
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
int fat32_open(const char *path, struct fuse_file_info *fileInfo) {
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
int fat32_read(const char *path, char *output, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
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
int fat32_write(const char *path, const char *input, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
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
int fat32_flush(const char *path, struct fuse_file_info *fileInfo) {
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
int fat32_release(const char *path, struct fuse_file_info *fileInfo) {
	//FIXME: implementar
	return -1;
}

/** Change the size of a file */
int fat32_truncate(const char *path, off_t newSize) {
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
int fat32_readdir(const char *path, void *output, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
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
int fat32_rename(const char *currentPath, const char *newPath) {
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
			configuracion configuracion;
			abrirArchivoDisco(&configuracion);
		    fat32_sector bootSector;
			leerSector(1, &bootSector, &configuracion);
			struct fat32_config_boot_sector configBootSector;
			leerBootSector(bootSector, &configBootSector );

			printf("Sector size: %" PRId16, &configBootSector.sectorSize);
			printf("Cluster Size: %s", &configBootSector.clusterSize);
			printf("bootRecordOffset: %" PRId32, &configBootSector.bootRecordOffset);
			printf("fatOffset: %" PRId16, &configBootSector.fatOffset);
			printf("Sector Count: %"PRId32,&configBootSector.sectorCount);
			printf("Fat Size: %"PRId32,&configBootSector.fatSize);
			printf("Fat Version: %"PRId16,&configBootSector.fatVersion);
			printf("rootDirectory: %"PRId32,&configBootSector.rootDirectory);
			printf("File System Info Sector: %"PRId16,&configBootSector.fileSystemInfoSector);
			printf("boot Sector Backup: %"PRId16,&configBootSector.bootSectorBackup);

			close(configuracion.archivoDisco);
	return fuse_main(argc, argv, &fat32_operations, NULL);
}
