import os
import tornado.ioloop
import tornado.web
import socket

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("Hello, world")
        
    def post(self):
        sender_name = self.get_argument("sender_name")
        file_path = self.get_argument("file_path")
        file_name = self.request.files["file"][0]["filename"]
        file_contents = self.request.files["file"][0]["body"]
        full_path = os.path.join(file_path, file_name)
        with open(full_path, "w") as f:
            f.write(file_contents.decode())
        self.write(f"File '{file_name}' uploaded by '{sender_name}' to '{full_path}'")

def make_app():
    return tornado.web.Application([
        (r"/", MainHandler),
    ])

if __name__ == "__main__":
    app = make_app()
    port = 8888
    app.listen(port)
    print(f"Web server started at {socket.gethostbyname(socket.gethostname())}:{port}")
    tornado.ioloop.IOLoop.current().start()
