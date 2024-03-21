import socket
import pygame

pygame.init()
pygame.display.set_caption("Attitude Indicator")
size = width, height = 500, 500
bgColor = 255, 255, 255
FPS = 60
scale_factor = 7

screen = pygame.display.set_mode(size)
frame = pygame.image.load(".\Frame.png").convert_alpha()
inel = pygame.image.load(".\Ring.png").convert_alpha()
interior = pygame.image.load(".\Interior.png").convert_alpha()

def blitRotateCenter(surf, image, topleft, angle):
    rotated_image = pygame.transform.rotate(image, angle)
    new_rect = rotated_image.get_rect(center = image.get_rect(topleft = topleft).center)
    surf.blit(rotated_image, new_rect)

def draw_window(set_pitch, set_roll, scale_factor = 1):
    screen.fill(bgColor)
    blitRotateCenter(screen, interior, (-width/2, -height/2 + set_pitch*scale_factor), set_roll)
    blitRotateCenter(screen, inel, (0, 0), set_roll)
    screen.blit(frame, (0,0))
    pygame.display.update()


if __name__ == '__main__':
    host = "192.168.1.143"  # as both code is running on same pc
    port = 8000  # socket server port number

    client_socket = socket.socket()  # instantiate
    client_socket.connect((host, port))  # connect to the server

    message = input(" -> ")  # take input

    while message.lower().strip() != 'bye':
        client_socket.send(message.encode())  # send message
        data = client_socket.recv(1024).decode()  # receive response
        valuesComplete = data.split("a")
        values = valuesComplete[0].split(";")
        if len(values) == 13:
            print('Roll ' + values[12] + ' Pitch ' + values[11])
            draw_window(float(values[12]), float(values[11]), scale_factor)
        elif len(values) == 2:
            print('Roll ' + values[1] + ' Pitch ' + values[0])
            draw_window(float(values[1]), float(values[0]), scale_factor)


    client_socket.close()  # close the connection
