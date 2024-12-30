/*
 * CPHTTP Library - A CPHTTP Client with Python Integration
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
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::string* response = (std::string*)userp;
    size_t total_size = size * nmemb;
    response->append((char*)contents, total_size);
    return total_size;
}

class Session {
public:
    CURL *curl;
    char *user_agent;
    char *proxy;
    char *cookie_file;
    char *ssl_cert;
    char *ssl_key;
    long timeout;
    std::string response_data;

    Session() 
        : curl(curl_easy_init()), user_agent(nullptr), proxy(nullptr),
          cookie_file(nullptr), ssl_cert(nullptr), ssl_key(nullptr), timeout(0) {}

    ~Session() {
        curl_easy_cleanup(curl);
        free(user_agent);
        free(proxy);
        free(cookie_file);
        free(ssl_cert);
        free(ssl_key);
    }

    void setUserAgent(const char* agent) {
        if (user_agent) free(user_agent);
        user_agent = strdup(agent);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);
    }

    void setProxy(const char* proxy) {
        if (this->proxy) free(this->proxy);
        this->proxy = strdup(proxy);
        curl_easy_setopt(curl, CURLOPT_PROXY, this->proxy);
    }

    void setCookieFile(const char* cookie_file) {
        if (this->cookie_file) free(this->cookie_file);
        this->cookie_file = strdup(cookie_file);
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, this->cookie_file);
    }

    void setSslCert(const char* ssl_cert) {
        if (this->ssl_cert) free(this->ssl_cert);
        this->ssl_cert = strdup(ssl_cert);
        curl_easy_setopt(curl, CURLOPT_SSLCERT, this->ssl_cert);
    }

    void setSslKey(const char* ssl_key) {
        if (this->ssl_key) free(this->ssl_key);
        this->ssl_key = strdup(ssl_key);
        curl_easy_setopt(curl, CURLOPT_SSLKEY, this->ssl_key);
    }

    void setTimeout(long timeout) {
        this->timeout = timeout;
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    }

    PyObject* httpGet(const char* url) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) throw std::runtime_error(curl_easy_strerror(res));

        return PyUnicode_FromString(response_data.c_str());
    }

    PyObject* httpPost(const char* url, const char* data) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) throw std::runtime_error(curl_easy_strerror(res));

        return PyUnicode_FromString(response_data.c_str());
    }

    PyObject* httpPut(const char* url, const char* data) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) throw std::runtime_error(curl_easy_strerror(res));

        return PyUnicode_FromString(response_data.c_str());
    }

    PyObject* httpDelete(const char* url) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) throw std::runtime_error(curl_easy_strerror(res));

        return PyUnicode_FromString(response_data.c_str());
    }

    PyObject* httpHead(const char* url) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) throw std::runtime_error(curl_easy_strerror(res));

        return PyUnicode_FromString(response_data.c_str());
    }
};

static void session_destructor(PyObject *capsule) {
    Session *session = (Session *)PyCapsule_GetPointer(capsule, "Session");
    delete session;
}

static PyObject* create_session(PyObject* self, PyObject* args) {
    Session* session = new Session();
    return PyCapsule_New(session, "Session", session_destructor);
}

static Session* get_session_from_capsule(PyObject* capsule) {
    return (Session*)PyCapsule_GetPointer(capsule, "Session");
}

static PyObject* set_user_agent(PyObject* self, PyObject* args) {
    PyObject* capsule;
    const char* agent;
    if (!PyArg_ParseTuple(args, "Os", &capsule, &agent)) return NULL;
    Session* session = get_session_from_capsule(capsule);
    if (!session) return NULL;
    try {
        session->setUserAgent(agent);
    } catch (const std::exception& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject* set_proxy(PyObject* self, PyObject* args) {
    PyObject* capsule;
    const char* proxy;
    if (!PyArg_ParseTuple(args, "Os", &capsule, &proxy)) return NULL;
    Session* session = get_session_from_capsule(capsule);
    if (!session) return NULL;
    try {
        session->setProxy(proxy);
    } catch (const std::exception& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject* set_cookie_file(PyObject* self, PyObject* args) {
    PyObject* capsule;
    const char* cookie_file;
    if (!PyArg_ParseTuple(args, "Os", &capsule, &cookie_file)) return NULL;
    Session* session = get_session_from_capsule(capsule);
    if (!session) return NULL;
    try {
        session->setCookieFile(cookie_file);
    } catch (const std::exception& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject* set_ssl_cert(PyObject* self, PyObject* args) {
    PyObject* capsule;
    const char* ssl_cert;
    if (!PyArg_ParseTuple(args, "Os", &capsule, &ssl_cert)) return NULL;
    Session* session = get_session_from_capsule(capsule);
    if (!session) return NULL;
    try {
        session->setSslCert(ssl_cert);
    } catch (const std::exception& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject* set_ssl_key(PyObject* self, PyObject* args) {
    PyObject* capsule;
    const char* ssl_key;
    if (!PyArg_ParseTuple(args, "Os", &capsule, &ssl_key)) return NULL;
    Session* session = get_session_from_capsule(capsule);
    if (!session) return NULL;
    try {
        session->setSslKey(ssl_key);
    } catch (const std::exception& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject* set_timeout(PyObject* self, PyObject* args) {
    PyObject* capsule;
    long timeout;
    if (!PyArg_ParseTuple(args, "Ol", &capsule, &timeout)) return NULL;
    Session* session = get_session_from_capsule(capsule);
    if (!session) return NULL;
    try {
        session->setTimeout(timeout);
    } catch (const std::exception& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject* http_get(PyObject* self, PyObject* args) {
    PyObject* capsule;
    const char* url;
    if (!PyArg_ParseTuple(args, "Os", &capsule, &url)) return NULL;
    Session* session = get_session_from_capsule(capsule);
    if (!session) return NULL;
    try {
        return session->httpGet(url);
    } catch (const std::exception& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
}

static PyObject* http_post(PyObject* self, PyObject* args) {
    PyObject* capsule;
    const char* url;
    const char* data;
    if (!PyArg_ParseTuple(args, "Oss", &capsule, &url, &data)) return NULL;
    Session* session = get_session_from_capsule(capsule);
    if (!session) return NULL;
    try {
        return session->httpPost(url, data);
    } catch (const std::exception& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
}

static PyObject* http_put(PyObject* self, PyObject* args) {
    PyObject* capsule;
    const char* url;
    const char* data;
    if (!PyArg_ParseTuple(args, "Oss", &capsule, &url, &data)) return NULL;
    Session* session = get_session_from_capsule(capsule);
    if (!session) return NULL;
    try {
        return session->httpPut(url, data);
    } catch (const std::exception& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
}

static PyObject* http_delete(PyObject* self, PyObject* args) {
    PyObject* capsule;
    const char* url;
    if (!PyArg_ParseTuple(args, "Os", &capsule, &url)) return NULL;
    Session* session = get_session_from_capsule(capsule);
    if (!session) return NULL;
    try {
        return session->httpDelete(url);
    } catch (const std::exception& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
}

static PyObject* http_head(PyObject* self, PyObject* args) {
    PyObject* capsule;
    const char* url;
    if (!PyArg_ParseTuple(args, "Os", &capsule, &url)) return NULL;
    Session* session = get_session_from_capsule(capsule);
    if (!session) return NULL;
    try {
        return session->httpHead(url);
    } catch (const std::exception& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
}

static PyMethodDef HttpRequestMethods[] = {
    {"create_session", create_session, METH_NOARGS, "Create a new session."},
    {"set_user_agent", set_user_agent, METH_VARARGS, "Set user agent."},
    {"set_proxy", set_proxy, METH_VARARGS, "Set proxy."},
    {"set_cookie_file", set_cookie_file, METH_VARARGS, "Set cookie file."},
    {"set_ssl_cert", set_ssl_cert, METH_VARARGS, "Set SSL certificate."},
    {"set_ssl_key", set_ssl_key, METH_VARARGS, "Set SSL key."},
    {"set_timeout", set_timeout, METH_VARARGS, "Set timeout."},
    {"http_get", http_get, METH_VARARGS, "Perform an HTTP GET request."},
    {"http_post", http_post, METH_VARARGS, "Perform an HTTP POST request."},
    {"http_put", http_put, METH_VARARGS, "Perform an HTTP PUT request."},
    {"http_delete", http_delete, METH_VARARGS, "Perform an HTTP DELETE request."},
    {"http_head", http_head, METH_VARARGS, "Perform an HTTP HEAD request."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef http_request_module = {
    PyModuleDef_HEAD_INIT,
    "CPHTTP",
    NULL,
    -1,
    HttpRequestMethods
};

PyMODINIT_FUNC PyInit_CPHTTP(void) {
    return PyModule_Create(&http_request_module);
}
