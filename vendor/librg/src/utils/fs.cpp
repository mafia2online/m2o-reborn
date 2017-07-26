// Copyright ReGuider Team, 2016-2017
//
#include <string.h>

#include <librg/core.h>
#include <librg/utils/fs.h>
#include <fcntl.h>

using namespace librg;

/**
 * Inner method
 * @param req
 */
void _onread(uv_fs_t *req)
{
    uv_fs_req_cleanup(req);

    if (req->result < 0) {
        core::error("file reading: %s\n", uv_strerror((int)req->result));
    }

    fs::result_t* result = (fs::result_t*) req->data;

    if (req->result > 0) {
        // result
        // result->content[result->length] = '\0';
        result->callback(result);
    }

    // cleaning up
    uv_fs_close(uv_default_loop(), req, result->handle, NULL);

    // deleting
    delete req;
    delete result->content;
    delete result->filepath;
    delete result;
}

/**
 * Inner method
 * @param req
 */
void _onwrite(uv_fs_t *req)
{
    uv_fs_req_cleanup(req);

    if (req->result < 0) {
        core::error("file writing: %s\n", uv_strerror((int)req->result));
    }

    fs::result_t* result = (fs::result_t*) req->data;

    if (req->result > 0) {
        // result
        // result->content[result->length] = '\0';
        result->callback(result);
    }

    // cleaning up
    uv_fs_close(uv_default_loop(), req, result->handle, NULL);

    // deleting
    delete req;
    delete result->filepath;
    delete result;
}

/**
 * Makedir function
 * @param  filename
 * @return result code (uv/system status codes)
 */
int fs::mkdir(std::string filename)
{
    uv_fs_t req;
    return uv_fs_mkdir(uv_default_loop(), &req, filename.c_str(), 0755, NULL);
}

/**
 * Removedir function
 * @param  filename
 * @return result code (uv/system status codes)
 */
int fs::rmdir(std::string filename)
{
    uv_fs_t req;
    return uv_fs_rmdir(uv_default_loop(), &req, filename.c_str(), NULL);
}



/**
 * Read file contents and pass result
 * of type fs:result_t to the callback
 * @param  filename
 * @param  callback
 * @return result of operation
 */
bool fs::read(std::string filename, fs::callback callback)
{
    uv_fs_t* req = new uv_fs_t;

    // trying to open file
    int handle = uv_fs_open(uv_default_loop(), req, filename.c_str(), 0, 0, NULL);

    if (handle < 0) {
        core::error("file reading: \"%s\" %s\n", filename.c_str(), uv_strerror(handle));
        return false;
    }

    // getting file size
    uv_fs_fstat(uv_default_loop(), req, handle, NULL);

    // create string
    char* str = new char[req->statbuf.st_size];

    // get the filename stuff
    char* filepath = new char[filename.size()];
    memcpy(filepath, filename.c_str(), filename.size());

    // creating content buffer
    result_t* data = new result_t;
    data->handle      = handle;
    data->length      = req->statbuf.st_size;
    data->content     = str;
    data->filepath    = filepath;
    data->fplength    = filename.size();
    data->callback    = callback;

    req->data = data;
    uv_buf_t uvBuf = uv_buf_init(str, (uint32_t)req->statbuf.st_size);

    uv_fs_read(uv_default_loop(), req, handle, &uvBuf, 1, -1, _onread);
    return true;
}

bool fs::write(std::string filename, size_t dataSize, byte *data, fs::callback callback)
{
    uv_fs_t* req = new uv_fs_t;

    // trying to open file
    int handle = uv_fs_open(uv_default_loop(), req, filename.c_str(),
                            O_CREAT|O_WRONLY|O_TRUNC, 0644, NULL);

    if (handle < 0) {
        core::error("file writing: \"%s\" %s\n", filename.c_str(), uv_strerror(handle));
        return false;
    }

    // get the filename stuff
    char* filepath = new char[filename.size()];
    memcpy(filepath, filename.c_str(), filename.size());

    uv_buf_t uvBuf = uv_buf_init(new char[dataSize], (uint32_t)dataSize);
    memcpy(uvBuf.base, data, dataSize);

    // creating content buffer
    result_t* buf    = new result_t;
    buf->handle      = handle;
    buf->length      = dataSize;
    buf->content     = uvBuf.base;
    buf->filepath    = filepath;
    buf->fplength    = filename.size();
    buf->callback    = callback;

    req->data = buf;

    uv_fs_write(uv_default_loop(), req, handle, &uvBuf, 1, -1, _onwrite);
    return true;
}
