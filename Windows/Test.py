from CHTTPClient import CHTTPClient
from HTTPCore import CHTTP
import time

def test_session():
    client = CHTTPClient(CHTTP)
    
    client.set_user_agent("MyCustomUserAgent/1.0")
    client.set_timeout(60)

    start_time = time.time()  
    
    try:
        
        get_response = client.http_get("http://example.com")
        print("GET Response:", get_response)

        
        post_response = client.http_post("http://example.com/api", {"key": "value"})
        print("POST Response:", post_response)

        
        put_response = client.http_put("http://example.com/api/1", {"key": "new_value"})
        print("PUT Response:", put_response)

        
        delete_response = client.http_delete("http://example.com/api/1")
        print("DELETE Response:", delete_response)

        
        head_response = client.http_head("http://example.com")
        print("HEAD Response:", head_response)
        
        client.reset()
        print("Client reset.")
    finally:
        client.close()

    end_time = time.time()  
    print("Total Execution Time:", end_time - start_time)  

if __name__ == "__main__":
    test_session()
