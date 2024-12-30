/*
 * CPHTTP Library - A CHTTP Client with Python Integration
 * Copyright (c) 2024 Sepehr0Day
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <Python.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    CURL *curl;
    char *user_agent;
    char *proxy;
    char *cookie_file;
    char *ssl_cert;
    char *ssl_key;
    long timeout;
    char response_buffer[16384];
} Session;

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    Session *session = (Session *)userp;

    if (total_size < sizeof(session->response_buffer) - 1) {
        strncat(session->response_buffer, contents, total_size);
        return total_size;
    }
    return 0;
}

static void session_destructor(PyObject *capsule) {
    Session *session = (Session *)PyCapsule_GetPointer(capsule, "Session");
    if (session) {
        curl_easy_cleanup(session->curl);
        free(session->user_agent);
        free(session->proxy);
        free(session->cookie_file);
        free(session->ssl_cert);
        free(session->ssl_key);
        free(session);
    }
}

static PyObject* create_session(PyObject* self, PyObject* args) {
    Session *session = (Session *)malloc(sizeof(Session));
    if (session == NULL) {
        PyErr_NoMemory();
        return NULL;
    }

    session->curl = curl_easy_init();
    session->user_agent = NULL;
    session->proxy = NULL;
    session->cookie_file = NULL;
    session->ssl_cert = NULL;
    session->ssl_key = NULL;
    session->timeout = 0;
    memset(session->response_buffer, 0, sizeof(session->response_buffer)); 

    curl_easy_setopt(session->curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(session->curl, CURLOPT_WRITEDATA, session);

    PyObject *capsule = PyCapsule_New(session, "Session", session_destructor);
    return capsule;
}

static PyObject* Session_set_user_agent(PyObject* self, PyObject* args) {
    PyObject *capsule;
    const char *agent;

    if (!PyArg_ParseTuple(args, "Os", &capsule, &agent)) {
        return NULL;
    }

    Session *session = (Session *)PyCapsule_GetPointer(capsule, "Session");
    if (session == NULL) {
        return NULL;
    }

    if (session->user_agent != NULL) {
        free(session->user_agent);
    }

    session->user_agent = strdup(agent);
    curl_easy_setopt(session->curl, CURLOPT_USERAGENT, session->user_agent);

    Py_RETURN_NONE;
}

static PyObject* Session_set_proxy(PyObject* self, PyObject* args) {
    PyObject *capsule;
    const char *proxy;

    if (!PyArg_ParseTuple(args, "Os", &capsule, &proxy)) {
        return NULL;
    }

    Session *session = (Session *)PyCapsule_GetPointer(capsule, "Session");
    if (session == NULL) {
        return NULL;
    }

    if (session->proxy) {
        free(session->proxy);
    }
    session->proxy = strdup(proxy);
    curl_easy_setopt(session->curl, CURLOPT_PROXY, session->proxy);

    Py_RETURN_NONE;
}

static PyObject* Session_set_cookie_file(PyObject* self, PyObject* args) {
    PyObject *capsule;
    const char *cookie_file;

    if (!PyArg_ParseTuple(args, "Os", &capsule, &cookie_file)) {
        return NULL;
    }

    Session *session = (Session *)PyCapsule_GetPointer(capsule, "Session");
    if (session == NULL) {
        return NULL;
    }

    if (session->cookie_file) {
        free(session->cookie_file);
    }
    session->cookie_file = strdup(cookie_file);
    curl_easy_setopt(session->curl, CURLOPT_COOKIEJAR, session->cookie_file);

    Py_RETURN_NONE;
}

static PyObject* Session_set_ssl_cert(PyObject* self, PyObject* args) {
    PyObject *capsule;
    const char *ssl_cert;

    if (!PyArg_ParseTuple(args, "Os", &capsule, &ssl_cert)) {
        return NULL;
    }

    Session *session = (Session *)PyCapsule_GetPointer(capsule, "Session");
    if (session == NULL) {
        return NULL;
    }

    if (session->ssl_cert) {
        free(session->ssl_cert);
    }
    session->ssl_cert = strdup(ssl_cert);
    curl_easy_setopt(session->curl, CURLOPT_SSLCERT, session->ssl_cert);

    Py_RETURN_NONE;
}

static PyObject* Session_set_ssl_key(PyObject* self, PyObject* args) {
    PyObject *capsule;
    const char *ssl_key;

    if (!PyArg_ParseTuple(args, "Os", &capsule, &ssl_key)) {
        return NULL;
    }

    Session *session = (Session *)PyCapsule_GetPointer(capsule, "Session");
    if (session == NULL) {
        return NULL;
    }

    if (session->ssl_key) {
        free(session->ssl_key);
    }
    session->ssl_key = strdup(ssl_key);
    curl_easy_setopt(session->curl, CURLOPT_SSLKEY, session->ssl_key);

    Py_RETURN_NONE;
}

static PyObject* Session_set_timeout(PyObject* self, PyObject* args) {
    PyObject *capsule;
    long timeout;

    if (!PyArg_ParseTuple(args, "Ol", &capsule, &timeout)) {
        return NULL;
    }

    Session *session = (Session *)PyCapsule_GetPointer(capsule, "Session");
    if (session == NULL) {
        return NULL;
    }

    session->timeout = timeout;
    curl_easy_setopt(session->curl, CURLOPT_TIMEOUT, session->timeout);

    Py_RETURN_NONE;
}

static PyObject* Session_http_get(PyObject* self, PyObject* args) {
    PyObject *capsule;
    const char *url;

    if (!PyArg_ParseTuple(args, "Os", &capsule, &url)) {
        return NULL;
    }

    Session *session = (Session *)PyCapsule_GetPointer(capsule, "Session");
    if (session == NULL) {
        return NULL;
    }

    curl_easy_setopt(session->curl, CURLOPT_URL, url);
    memset(session->response_buffer, 0, sizeof(session->response_buffer));

    CURLcode res = curl_easy_perform(session->curl);
    if (res != CURLE_OK) {
        PyErr_SetString(PyExc_RuntimeError, curl_easy_strerror(res));
        return NULL;
    }

    return Py_BuildValue("s", session->response_buffer);
}

static PyObject* Session_http_post(PyObject* self, PyObject* args) {
    PyObject *capsule;
    const char *url;
    const char *data;

    if (!PyArg_ParseTuple(args, "Oss", &capsule, &url, &data)) {
        return NULL;
    }

    Session *session = (Session *)PyCapsule_GetPointer(capsule, "Session");
    if (session == NULL) {
        return NULL;
    }

    curl_easy_setopt(session->curl, CURLOPT_URL, url);
    curl_easy_setopt(session->curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(session->curl, CURLOPT_POSTFIELDS, data);
    memset(session->response_buffer, 0, sizeof(session->response_buffer));

    CURLcode res = curl_easy_perform(session->curl);
    if (res != CURLE_OK) {
        PyErr_SetString(PyExc_RuntimeError, curl_easy_strerror(res));
        return NULL;
    }

    return Py_BuildValue("s", session->response_buffer);
}

static PyObject* Session_http_put(PyObject* self, PyObject* args) {
    PyObject *capsule;
    const char *url;
    const char *data;

    if (!PyArg_ParseTuple(args, "Oss", &capsule, &url, &data)) {
        return NULL;
    }

    Session *session = (Session *)PyCapsule_GetPointer(capsule, "Session");
    if (session == NULL) {
        return NULL;
    }

    curl_easy_setopt(session->curl, CURLOPT_URL, url);
    curl_easy_setopt(session->curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(session->curl, CURLOPT_POSTFIELDS, data);
    memset(session->response_buffer, 0, sizeof(session->response_buffer));

    CURLcode res = curl_easy_perform(session->curl);
    if (res != CURLE_OK) {
        PyErr_SetString(PyExc_RuntimeError, curl_easy_strerror(res));
        return NULL;
    }

    return Py_BuildValue("s", session->response_buffer);
}

static PyObject* Session_http_delete(PyObject* self, PyObject* args) {
    PyObject *capsule;
    const char *url;

    if (!PyArg_ParseTuple(args, "Os", &capsule, &url)) {
        return NULL;
    }

    Session *session = (Session *)PyCapsule_GetPointer(capsule, "Session");
    if (session == NULL) {
        return NULL;
    }

    curl_easy_setopt(session->curl, CURLOPT_URL, url);
    curl_easy_setopt(session->curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    memset(session->response_buffer, 0, sizeof(session->response_buffer));

    CURLcode res = curl_easy_perform(session->curl);
    if (res != CURLE_OK) {
        PyErr_SetString(PyExc_RuntimeError, curl_easy_strerror(res));
        return NULL;
    }

    return Py_BuildValue("s", session->response_buffer);
}

static PyObject* Session_http_head(PyObject* self, PyObject* args) {
    PyObject *capsule;
    const char *url;

    if (!PyArg_ParseTuple(args, "Os", &capsule, &url)) {
        return NULL;
    }

    Session *session = (Session *)PyCapsule_GetPointer(capsule, "Session");
    if (session == NULL) {
        return NULL;
    }

    curl_easy_setopt(session->curl, CURLOPT_URL, url);
    curl_easy_setopt(session->curl, CURLOPT_NOBODY, 1L);
    memset(session->response_buffer, 0, sizeof(session->response_buffer));

    CURLcode res = curl_easy_perform(session->curl);
    if (res != CURLE_OK) {
        PyErr_SetString(PyExc_RuntimeError, curl_easy_strerror(res));
        return NULL;
    }

    return Py_BuildValue("s", session->response_buffer);
}

static PyMethodDef HttpRequestMethods[] = {
    {"create_session", create_session, METH_NOARGS, "Create a new session."},
    {"set_user_agent", Session_set_user_agent, METH_VARARGS, "Set user agent."},
    {"set_proxy", Session_set_proxy, METH_VARARGS, "Set proxy."},
    {"set_cookie_file", Session_set_cookie_file, METH_VARARGS, "Set cookie file."},
    {"set_ssl_cert", Session_set_ssl_cert, METH_VARARGS, "Set SSL certificate."},
    {"set_ssl_key", Session_set_ssl_key, METH_VARARGS, "Set SSL key."},
    {"set_timeout", Session_set_timeout, METH_VARARGS, "Set timeout."},
    {"http_get", Session_http_get, METH_VARARGS, "Perform an HTTP GET request."},
    {"http_post", Session_http_post, METH_VARARGS, "Perform an HTTP POST request."},
    {"http_put", Session_http_put, METH_VARARGS, "Perform an HTTP PUT request."},
    {"http_delete", Session_http_delete, METH_VARARGS, "Perform an HTTP DELETE request."},
    {"http_head", Session_http_head, METH_VARARGS, "Perform an HTTP HEAD request."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef http_request_module = {
    PyModuleDef_HEAD_INIT,
    "CHTTP",
    NULL,
    -1,
    HttpRequestMethods
};

PyMODINIT_FUNC PyInit_CHTTP(void) {
    return PyModule_Create(&http_request_module);
}
