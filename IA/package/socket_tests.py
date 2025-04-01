import socket
from typing import Any
import pytest
import threading
from .AI import AI
from .zappy_ai import run_AI


SERVER_HOST: tuple[str, int] = ("localhost", 4242)


def handle_client(client_socket: socket.socket, client_address: socket.socket):
    while True:
        try:
            # Receive data from client
            message = client_socket.recv(1024).decode('utf-8')
            if not message:
                break
            print(f"Received from {client_address}: {message}")

            # Send response back to client
            response = f"Echo: {message}"
            client_socket.send(response.encode('utf-8'))
        except ConnectionResetError:
            break


def start_serv():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(SERVER_HOST)
    sock.listen()

    while True:
        client_socket, client_address = sock.accept()
        client_handler = threading.Thread(target=handle_client, args=(client_socket, client_address))
        client_handler.start()


def empty(*args: Any, **kwargs: Any) -> None:
    pass


def nope(*args: Any, **kwargs: Any) -> None:
    raise SystemError("monkeypatch blocked")


@pytest.mark.Connection
@pytest.hookimpl(tryfirst=True)
@pytest.mark.usefixtures("monkeypatch")
def test_socket_connection(monkeypatch: pytest.MonkeyPatch):
    serv = threading.Thread(target=start_serv)
    serv.run()
    monkeypatch.setattr(AI, "__init__", nope)
    connection = socket.create_connection(SERVER_HOST)
    with pytest.raises(SystemError):
        run_AI(connection)
