from HTTPCore import CPHTTP
import json

class CPHTTPClient:
    def __init__(self):
        """
        Initializes the CPHTTPClient instance by creating a session.

        Raises:
            Exception: If the session creation fails.
        """
        self.capsule = CPHTTP.create_session()
        if self.capsule is None:
            raise Exception("Failed to create session.")
        
        self.default_user_agent = None
        self.default_proxy = None
        self.default_cookie_file = None
        self.default_ssl_cert = None
        self.default_ssl_key = None
        self.default_timeout = None

    def set_user_agent(self, user_agent):
        """
        Sets the User-Agent header for HTTP requests.

        Parameters:
            user_agent (str): The User-Agent string to be used in HTTP requests.

        Example:
            client.set_user_agent("MyCustomUserAgent/1.0")
        """
        CPHTTP.set_user_agent(self.capsule, user_agent)
        self.default_user_agent = user_agent

    def set_proxy(self, proxy_url):
        """
        Sets the proxy URL for HTTP requests.

        Parameters:
            proxy_url (str): The URL of the proxy server.

        Example:
            client.set_proxy("http://proxy.example.com:8080")
        """
        CPHTTP.set_proxy(self.capsule, proxy_url)
        self.default_proxy = proxy_url

    def set_cookie_file(self, cookie_file_path):
        """
        Sets the path to the cookie file for HTTP requests.

        Parameters:
            cookie_file_path (str): The path to the cookie file.

        Example:
            client.set_cookie_file("/path/to/cookiefile")
        """
        CPHTTP.set_cookie_file(self.capsule, cookie_file_path)
        self.default_cookie_file = cookie_file_path

    def set_ssl_cert(self, cert_file_path):
        """
        Sets the path to the SSL certificate file for secure HTTP connections.

        Parameters:
            cert_file_path (str): The path to the SSL certificate file.

        Example:
            client.set_ssl_cert("/path/to/cert.pem")
        """
        CPHTTP.set_ssl_cert(self.capsule, cert_file_path)
        self.default_ssl_cert = cert_file_path

    def set_ssl_key(self, key_file_path):
        """
        Sets the path to the SSL key file for secure HTTP connections.

        Parameters:
            key_file_path (str): The path to the SSL key file.

        Example:
            client.set_ssl_key("/path/to/key.pem")
        """
        CPHTTP.set_ssl_key(self.capsule, key_file_path)
        self.default_ssl_key = key_file_path

    def set_timeout(self, timeout_seconds):
        """
        Sets the timeout for HTTP requests.

        Parameters:
            timeout_seconds (int): The timeout duration in seconds.

        Example:
            client.set_timeout(60)
        """
        CPHTTP.set_timeout(self.capsule, timeout_seconds)
        self.default_timeout = timeout_seconds

    def reset(self):
        """
        Resets the CPHTTPClient to its default state by clearing all configurations.

        This method restores default settings for user agent, proxy, cookie file, SSL certificates, and timeout.

        Example:
            client.reset()
        """
        if self.default_user_agent:
            CPHTTP.set_user_agent(self.capsule, self.default_user_agent)
        if self.default_proxy:
            CPHTTP.set_proxy(self.capsule, self.default_proxy)
        if self.default_cookie_file:
            CPHTTP.set_cookie_file(self.capsule, self.default_cookie_file)
        if self.default_ssl_cert:
            CPHTTP.set_ssl_cert(self.capsule, self.default_ssl_cert)
        if self.default_ssl_key:
            CPHTTP.set_ssl_key(self.capsule, self.default_ssl_key)
        if self.default_timeout is not None:
            CPHTTP.set_timeout(self.capsule, self.default_timeout)

    def http_get(self, url):
        """
        Performs an HTTP GET request.

        Parameters:
            url (str): The URL for the GET request.

        Returns:
            str: The response from the server, or an error message if the request fails.

        Example:
            response = client.http_get("http://example.com")
        """
        try:
            response = CPHTTP.http_get(self.capsule, url)
            return response if response else "No response"
        except Exception as e:
            return f"Error: {str(e)}"

    def http_post(self, url, payload):
        """
        Performs an HTTP POST request.

        Parameters:
            url (str): The URL for the POST request.
            payload (dict or str): The payload for the POST request. If a dictionary is provided, it is converted to a JSON string.

        Returns:
            str: The response from the server, or an error message if the request fails.

        Example:
            payload = {"key": "value"}
            response = client.http_post("http://example.com/api", payload)
        """
        if isinstance(payload, dict):
            payload = json.dumps(payload)
        try:
            response = CPHTTP.http_post(self.capsule, url, payload)
            return response.decode() if response else "No response"
        except Exception as e:
            return f"Error: {str(e)}"

    def http_put(self, url, payload):
        """
        Performs an HTTP PUT request.

        Parameters:
            url (str): The URL for the PUT request.
            payload (dict or str): The payload for the PUT request. If a dictionary is provided, it is converted to a JSON string.

        Returns:
            str: The response from the server, or an error message if the request fails.

        Example:
            payload = {"key": "new_value"}
            response = client.http_put("http://example.com/api/1", payload)
        """
        if isinstance(payload, dict):
            payload = json.dumps(payload)
        try:
            response = CPHTTP.http_put(self.capsule, url, payload)
            return response.decode() if response else "No response"
        except Exception as e:
            return f"Error: {str(e)}"

    def http_delete(self, url):
        """
        Performs an HTTP DELETE request.

        Parameters:
            url (str): The URL for the DELETE request.

        Returns:
            str: The response from the server, or an error message if the request fails.

        Example:
            response = client.http_delete("http://example.com/api/1")
        """
        try:
            response = CPHTTP.http_delete(self.capsule, url)
            return response.decode() if response else "No response"
        except Exception as e:
            return f"Error: {str(e)}"

    def http_head(self, url):
        """
        Performs an HTTP HEAD request.

        Parameters:
            url (str): The URL for the HEAD request.

        Returns:
            str: The response from the server, or an error message if the request fails.

        Example:
            response = client.http_head("http://example.com")
        """
        try:
            response = CPHTTP.http_head(self.capsule, url)
            return response.decode() if response else "No response"
        except Exception as e:
            return f"Error: {str(e)}"

    def close(self):
        """
        Closes the HTTP session.

        Example:
            client.close()
        """
        self.capsule = None

# def test_session():
#     client = CPHTTPClient()
    
#     client.set_user_agent("MyCustomUserAgent/1.0")
#     client.set_timeout(60)
    
#     print("Performing HTTP GET request:")
#     print("Response:", client.http_get("http://example.com"))

#     print("Performing HTTP POST request:")
#     payload = {"key": "value"}
#     print("Response:", client.http_post("http://example.com/api", payload))
    
#     print("Performing HTTP PUT request:")
#     payload = {"key": "new_value"}
#     print("Response:", client.http_put("http://example.com/api/1", payload))
    
#     print("Performing HTTP DELETE request:")
#     print("Response:", client.http_delete("http://example.com/api/1"))
    
#     print("Performing HTTP HEAD request:")
#     print("Response:", client.http_head("http://example.com"))

#     client.reset()
#     print("Client reset.")
    
#     client.close()

# if __name__ == "__main__":
#     test_session()

client = CPHTTPClient()
    
client.set_user_agent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36")
client.set_timeout(60)

resp = client.http_get("https://google.com/")
print(resp)