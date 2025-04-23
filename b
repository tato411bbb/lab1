
web shell: https://blog.wasmer.io/webassembly-sh-408b010c14db

https://github.com/yudai/gotty


https://xtermjs.org/




https://github.com/sigoden/llm-functions

https://playground.ai.cloudflare.com/

https://github.com/sigoden/aichat

https://itsfoss.com/linux-terminal-ai/
shell genie.


https://github.com/BuilderIO/ai-shell
https://slashdot.org/software/p/Yai/alternatives



https://pypi.org/project/shell-gpt/0.1/#files

####################################################################################

import streamlit as st
import os

# Allowed file extensions
ALLOWED_EXTENSIONS = ['pptx', 'rtf', 'docx', 'xls', 'txt', 'pdf']

def save_uploaded_file(uploaded_file):
    save_path = os.path.join("/tmp", uploaded_file.name)
    with open(save_path, "wb") as f:
        f.write(uploaded_file.getbuffer())
    return save_path

def main():
    st.title("File Upload to /tmp")

    uploaded_file = st.file_uploader(
        "Upload a file",
        type=ALLOWED_EXTENSIONS,
        help="Allowed file types: .pptx, .rtf, .docx, .xls, .txt, .pdf"
    )

    if uploaded_file is not None:
        saved_path = save_uploaded_file(uploaded_file)
        st.success(f"File '{uploaded_file.name}' uploaded and saved to {saved_path}")

if __name__ == "__main__":
    main()
