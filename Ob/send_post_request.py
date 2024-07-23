import requests
import logging
from requests.adapters import HTTPAdapter
from requests.packages.urllib3.util.retry import Retry

# Configure logging
logging.basicConfig(level=logging.INFO)

url = 'http://localhost:8081'  # Replace with your server's URL
data = {
    'data': 'Thiuest'
}

# Function to send POST request
def send_post_request(url, data):
    try:
        # Set up session with retries
        session = requests.Session()
        retry = Retry(connect=3, backoff_factor=0.5)
        adapter = HTTPAdapter(max_retries=retry)
        session.mount('http://', adapter)
        session.mount('https://', adapter)

        logging.info('Sending POST request to %s with data: %s', url, data)
        response = session.post(url, data=data)

        logging.info('Status Code: %d', response.status_code)
        logging.info('Response Text: %s', response.text)
        return response

    except requests.exceptions.RequestException as e:
        logging.error('Request failed: %s', e)
        return None

# Send the POST request
response = send_post_request(url, data)
if response:
    print('Request sent successfully.')
else:
    print('Failed to send request.')
