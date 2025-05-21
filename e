python

import streamlit as st
import subprocess

st.title("Dynamic Shell Script Runner")

# Add input field for user text
user_input = st.text_input ("Enter input",
value="hello1")

if st.button("click here"):
try:

#

argument

Execute script with user input as

result = subprocess.run(
['/tmp/1.sh', user_input],
capture_output=True,
text=True,
check=True

)
st.success ("Output: ")
st.code(result.stdout)
except subprocess. CalledProcessError as

e:

st.error(f"Error: {e}\n{e.stderr}")


