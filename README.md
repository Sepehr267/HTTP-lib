# HTTP Client Library

A lightweight and cross-platform library designed to simplify HTTP requests and responses. This library is implemented in Python and C/C++ to combine ease of use with high performance.

---

## Features

- **Cross-Platform**: Compatible with Linux and Windows.
- **Easy-to-Use Interface**: Simplified Python API for sending HTTP requests.
- **Performance-Oriented**: Core C/C++ implementation for optimized performance.
- **Flexible**: Supports GET, POST, PUT, DELETE, and HEAD methods.
- **Open Source**: Feel free to use and modify.

---

## Installation
Just download the project and use the Python client file along with the compiled file alongside your project.

### Requirements

- Python 3.7 or later

## Usage

See `Linux/CHTTP.py` and `Linux/CPHTTP.py` for Linux examples, and `Windows/Test.py` for Windows examples.

### Windows (Python Example)

In Windows, the library supports HTTP operations through the `CHTTPClient` class.

```python
from CHTTPClient import CHTTPClient
from HTTPCore import CHTTP


def test_session():
    client = CHTTPClient(CHTTP)
    
    client.set_user_agent("MyCustomUserAgent/1.0")
    client.set_timeout(60)

    try:
        get_response = client.http_get("http://example.com")
        print("GET Response:", get_response)

        post_response = client.http_post("http://example.com/api", {"key": "value"})
        print("POST Response:", post_response)
    finally:
        client.close()

if __name__ == "__main__":
    test_session()
```

### Linux (Python Example)

In Linux, the library supports HTTP operations via the `CHTTP` module.

```python
from HTTPCore import CHTTP

def test_session():
    client = CHTTPClient()
    
    client.set_user_agent("MyCustomUserAgent/1.0")
    client.set_timeout(60)

    try:
        get_response = client.http_get("http://example.com")
        print("GET Response:", get_response)

        post_response = client.http_post("http://example.com/api", {"key": "value"})
        print("POST Response:", post_response)
    finally:
        client.close()

if __name__ == "__main__":
    test_session()
```
---

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your improvements or bug fixes.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## Contact

For any inquiries, feel free to reach out to:

- **Email**: [sphrz2324@gmail.com](mailto:sphrz2324@gmail.com)
- **Telegram**: [@Sepehr0Day](https://t.me/Sepehr0Day)

---

<br>

*If you enjoyed this project or found it useful, please consider giving it a star to support its development!* ⭐
