#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Menu.h"
using namespace std;
using namespace sf;

Event event;
bool sound = true;
bool hard = false;
bool new_user = false;
bool replay_lvl1 = false, replay_lvl2 = false, replay_lvl3 = false, replay_lvl4 = false;
int name_selected = 0;
int num_of_users;
int LEVEL_ALL_USERS[1000] = {};
const int MAX_PLAYERS = 5;
int PageNumber = 1000; //default => 1000
float volume = 1; // 100%
struct user {
    int level;
    string username;
}Data[MAX_PLAYERS + 1];

void read();
void edit_data();
void write();
void intro(RenderWindow& window); //PageNumber => 0
void playerName(RenderWindow& window, Sprite& background, Music& bg_music); //PageNumber => -4
void loadGame(RenderWindow& window, Sprite& background, Music& bg_music); //PageNumber => -5
void options(RenderWindow& window, Sprite& background, Music& bg_music); //PageNumber => -2
void controls(RenderWindow& window, Sprite& background); //PageNumber => -8
void loading(RenderWindow& window, int windowNum); //PageNumber => -3
void gameWindow1(RenderWindow& window); //PageNumber => 1
void gameWindow2(RenderWindow& window); //PageNumber => 2
void gameWindow3(RenderWindow& window, Music foot[], Music& shoot, Music& jump, Music& punch, Music& hurt, Music& music_bg, Music& game_over_sound, Texture& fire_t, Sprite& health1, Sprite& health2, Sprite& health3, Sprite& slot, Sprite& Time, Text& TIME, Sprite& pyramid, int& TIME_Display, Texture& timer_T); //PageNumber => 3
void gameWindow4(RenderWindow& window, Sprite& slot); //PageNumber => 4
void gameWindow5(RenderWindow& window); //PageNumber => 5
void winWindow(RenderWindow& window, int windowNum); //PageNumber => -7
void gameOver(RenderWindow& window, int windowNum); //PageNumber => -6
void pause(RenderWindow& window);


int main() {

    read();
    RenderWindow window(VideoMode(1920, 1080), "The Lost Relics", Style::Fullscreen);
    window.setFramerateLimit(60);
    Menu menu(1920, 1080);
    Image cursorImage;
    cursorImage.loadFromFile("./assets/textures/mouseCross.png");
    Cursor mouseCross;
    mouseCross.loadFromPixels(cursorImage.getPixelsPtr(), cursorImage.getSize(), { 0, 0 });
    window.setMouseCursor(mouseCross);

    Music bg_music;
    bg_music.openFromFile("./assets/sounds/Ancient Egyptian Music.mp3");
    bg_music.setVolume(25 * volume);
    bg_music.setLoop(true);

    //menu background
    Texture background_T;
    background_T.loadFromFile("./assets/textures/Untitled965 (01).png");
    Sprite background;
    background.setTexture(background_T);

    //sounds
    Music enter;
    enter.openFromFile("./assets/sounds/enter.mp3");
    enter.setVolume(50);

    Clock c;
    float deltaTime = 0, timer = 0;
    int B = 0, B2 = 0;

    while (true) {
        if (PageNumber == 1000) {
            while (window.isOpen()) {
                if (sound && (bg_music.getStatus() == Music::Stopped)) {
                    bg_music.play();
                }
                c.restart();
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed) {
                        window.close();
                        break;
                    }
                    if (event.type == Event::KeyPressed) {
                        if (event.key.code == Keyboard::Up) {
                            menu.MoveUp();
                        }
                    }
                    if (event.type == Event::KeyPressed) {
                        if (event.key.code == Keyboard::Down) {
                            menu.MoveDown();
                        }
                        if (event.key.code == Keyboard::Enter) {
                            enter.play();
                            if (menu.pressed() == 0) {
                                PageNumber = -4;
                            }
                            else if (menu.pressed() == 1) {
                                PageNumber = -5;
                            }
                            else if (menu.pressed() == 2) {
                                PageNumber = -8;
                            }
                            else if (menu.pressed() == 3) {
                                PageNumber = -2;
                            }
                            else if (menu.pressed() == 4) {
                                PageNumber = -1;
                            }
                        }
                    }
                }
                replay_lvl1 = false;
                replay_lvl2 = false;
                replay_lvl3 = false;
                replay_lvl4 = false;

                //mouse
                if (menu.mouse.getGlobalBounds().intersects(menu.mainmenu[0].getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)) {
                    enter.play();
                    PageNumber = -4;
                }
                else if (menu.mouse.getGlobalBounds().intersects(menu.mainmenu[1].getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)) {
                    enter.play();
                    PageNumber = -5;
                }
                else if (menu.mouse.getGlobalBounds().intersects(menu.mainmenu[2].getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)) {
                    enter.play();
                    PageNumber = -8;
                }
                else if (menu.mouse.getGlobalBounds().intersects(menu.mainmenu[3].getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)) {
                    enter.play();
                    PageNumber = -2;
                }
                else if (menu.mouse.getGlobalBounds().intersects(menu.mainmenu[4].getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)) {
                    enter.play();
                    PageNumber = -1;
                }

                //bg
                if (timer < 0) {
                    background.setTextureRect(IntRect(B * 1920, B2 * 1080, 1920, 1080));
                    B++;
                    B %= 9;
                    if (B == 8) {
                        B = 0;
                        B2++;
                    }
                    if (B2 == 7) {
                        B2 = 0;
                    }
                    timer = 0.066;
                }
                else {
                    timer -= deltaTime;
                }

                window.clear();
                window.draw(background);
                menu.draw(window);
                menu.Mouse(window);
                window.display();
                deltaTime = c.getElapsedTime().asSeconds();
                if (PageNumber != 1000) {
                    break;
                }

            }

            if (PageNumber == -1) {
                window.close();
                break;
            }
            else if (PageNumber == -5) {
                loadGame(window, background, bg_music);
            }
            else if (PageNumber == -4) {
                playerName(window, background, bg_music);
            }
            else if (PageNumber == -2) {
                options(window, background, bg_music);
            }
            else if (PageNumber == -8) {
                controls(window, background);
            }
        }

    }
    write();
    return 0;
}

void read() {
    ifstream read_users;
    read_users.open("savevariable.txt", ios::binary);
    read_users.seekg(0, ios::end);
    read_users.seekg(-static_cast<int>(sizeof(int)), ios::cur);
    read_users >> num_of_users;
    read_users.close();

    ifstream read_arr("data.txt");
    for (int i = 0; i < num_of_users; ++i) {
        read_arr >> Data[i].level;

    }
    read_arr.close();


    ifstream read_arr_string("history.txt");
    string name;
    int i = 0;
    while (getline(read_arr_string, name)) {
        Data[i].username = name;
        i++;
        if (i == num_of_users) { break; }
    }
    read_arr_string.close();
}

void edit_data() {
    if (new_user) {
        //?? ?????? ??? ???? ???? ????
        //   ????? ?????? ???? ??????? ??????
        Data[num_of_users - 1].level = PageNumber;
    }

    else {//?? ????? ??? ?? ???? ?????? ??? ??? ?? ??? ????
        //  ????? ?????? ???? ?????? 
        Data[name_selected].level = PageNumber;

    }
}

void write() {
    ofstream write_users;
    write_users.open("savevariable.txt", ios::app);
    write_users << num_of_users << endl;
    write_users.close();


    fstream write_arr;
    write_arr.open("data.txt", ios::in | ios::out);
    for (int i = 0; i < num_of_users; ++i) {
        write_arr << Data[i].level << endl;
    }
    write_arr.close();

    fstream write_arr_string;
    write_arr_string.open("history.txt", ios::in | ios::out);
    for (int i = 0; i < num_of_users; ++i) {
        write_arr_string << Data[i].username << endl;
    }
    write_arr_string.close();
}

void playerName(RenderWindow& window, Sprite& background, Music& bg_music) {
    if (!Data[num_of_users].username.empty()) {
        Data[num_of_users].username.clear();
    }
    RectangleShape slide;
    slide.setFillColor(Color{ 255,255,255,140 });
    RectangleShape shape(Vector2f(1920, 1080));
    shape.setFillColor(Color(0, 0, 0, 122));
    Music keyboard;
    keyboard.openFromFile("./assets/sounds/keyboard.mp3");
    keyboard.setVolume(30);

    Music error;
    error.openFromFile("./assets/sounds/error.mp3");
    error.setVolume(100);

    Music enter;
    enter.openFromFile("./assets/sounds/enter.mp3");
    enter.setVolume(50);

    Music click;
    click.openFromFile("./assets/sounds/click.mp3");
    click.setVolume(50);

    //menu background
    Texture backgroundd_TT;
    backgroundd_TT.loadFromFile("./assets/textures/Untitled966.png");
    Sprite backgroundd;
    backgroundd.setTexture(backgroundd_TT);

    Texture background_enter_T;
    background_enter_T.loadFromFile("./assets/textures/Untitled967.png");
    Sprite background_enter;
    background_enter.setTexture(background_enter_T);
    Font font;
    font.loadFromFile("./assets/font/HeadlinerNo.45 DEMO.ttf");

    Texture esc_t;
    esc_t.loadFromFile("./assets/textures/esc.png");
    Sprite esc;
    esc.setTexture(esc_t);
    Text ESC;
    ESC.setFont(font);
    ESC.setCharacterSize(37);
    ESC.setString("esc");
    ESC.setPosition(25, 6);


    Text Playername;
    Text ok;
    ok.setFont(font);
    ok.setFillColor(Color::White);
    ok.setCharacterSize(37);
    ok.setString("confirm");
    ok.setPosition(375, 590);

    Playername.setFont(font);
    //Playername.setFillColor(Color{ 139,48,48 }); ==> red
    Playername.setFillColor(Color{ 115,158,170 });
    Playername.setOutlineColor(Color::Black);
    Playername.setOutlineThickness(0.1f);
    Playername.setCharacterSize(65);
    Playername.setPosition(116, 491);

    RectangleShape mouse(Vector2f(15, 20));
    Texture error_messageT;
    error_messageT.loadFromFile("./assets/textures/Untitled9677.png");
    Sprite error_message;
    error_message.setTexture(error_messageT);
    bool Repeated_name = false;

    Clock c;
    float deltaTime = 0, timer = 0, timer2 = 0;
    int B = 0, B2 = 0, k = 0;
    while (window.isOpen()) {
        if (PageNumber != -4) break;
        bg_music.setVolume(25 * volume);
        c.restart();
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::TextEntered) {
                if (Keyboard::isKeyPressed(Keyboard::Backspace) && Data[num_of_users].username.size() == 0) {
                    error.play();
                }
                else if (Data[num_of_users].username.size() > 21) {
                    error.play();
                }
                else if (Keyboard::isKeyPressed(Keyboard::Enter)) { error.play(); }
                else {
                    if (static_cast<char>(event.text.unicode) >= 'A' && static_cast<char>(event.text.unicode) <= 'Z') {
                        keyboard.play();
                        Data[num_of_users].username += static_cast<char>(event.text.unicode);
                        Repeated_name = false;
                    }
                    else if (static_cast<char>(event.text.unicode) >= 'a' && static_cast<char>(event.text.unicode) <= 'z' || static_cast<char>(event.text.unicode) <= ' ') {
                        keyboard.play();
                        Data[num_of_users].username += static_cast<char>(event.text.unicode);
                        Repeated_name = false;
                    }
                    else if (static_cast<char>(event.text.unicode) >= '0' && static_cast<char>(event.text.unicode) <= '9') {
                        keyboard.play();
                        Data[num_of_users].username += static_cast<char>(event.text.unicode);
                        Repeated_name = false;
                    }
                }
            }

            if (Keyboard::isKeyPressed(Keyboard::Backspace) && Data[num_of_users].username.size() > 0) {
                Data[num_of_users].username.resize(Data[num_of_users].username.size() - 1);
                Repeated_name = false;
            }
            if (Keyboard::isKeyPressed(Keyboard::Enter) && Data[num_of_users].username.size() >= 1) {
                Repeated_name = false;
                for (int i = 0; i < num_of_users; i++) {
                    if (Data[num_of_users].username == Data[i].username) {
                        Repeated_name = true;
                        break;
                    }
                }
                if (Repeated_name == false) {
                    enter.play();
                    PageNumber = -3; //lowding window
                }
                else { error.play(); }

            }
            if (mouse.getGlobalBounds().intersects(ok.getGlobalBounds()) && Data[num_of_users].username.size() >= 1 && !Repeated_name) {
                ok.setFillColor(Color{ 115,158,170 });
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    Repeated_name = false;
                    for (int i = 0; i < num_of_users; i++) {
                        if (Data[num_of_users].username == Data[i].username) {
                            Repeated_name = true;
                            break;
                        }
                    }
                    if (Repeated_name == false) {
                        enter.play();
                        PageNumber = -3; //lowding window
                    }
                    else { error.play(); }
                }
            }

            else if (Data[num_of_users].username.size() < 1 || Repeated_name) {
                ok.setFillColor(Color{ 91,93,93,100 });
            }
            else {
                ok.setFillColor(Color::White);
            }
            if (PageNumber == -3) {

                num_of_users++;
                if (num_of_users > MAX_PLAYERS) {
                    for (int i = 0; i < MAX_PLAYERS; i++) {
                        Data[i].username = Data[i + 1].username;
                        Data[i].level = Data[i + 1].level;
                    }
                    num_of_users = MAX_PLAYERS;
                }

                new_user = true;
                bg_music.stop();
                loading(window, 0);
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                enter.play();
                sleep(milliseconds(150));
                PageNumber = 1000;
                return;
            }
            if (mouse.getGlobalBounds().intersects(ESC.getGlobalBounds())) {
                ESC.setFillColor(Color{ 115,158,170 });
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    enter.play();
                    sleep(milliseconds(150));
                    PageNumber = 1000;
                    return;
                }
            }
            else {
                ESC.setFillColor(Color::White);
            }
        }
        Vector2i mousePosition = Mouse::getPosition(window);
        Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);
        mouse.setPosition(worldMousePosition);

        if (timer < 0) {
            background.setTextureRect(IntRect(B * 1920, B2 * 1080, 1920, 1080));
            B++;
            B %= 9;
            if (B == 8) {
                B = 0;
                B2++;
            }
            if (B2 == 7) {
                B2 = 0;
            }
            timer = 0.066;
        }
        else {
            timer -= deltaTime;
        }

        if (timer2 < 0) {
            k++;
            timer2 = 0.5;
        }
        else {
            timer2 -= deltaTime;
        }

        if (k % 2 == 0) {
            slide.setSize(Vector2f(2, 53));
        }
        else {
            slide.setSize(Vector2f(0, 0));
        }

        slide.setPosition(Playername.getPosition().x + Playername.getGlobalBounds().width + 2, Playername.getPosition().y + 14);

        Playername.setString(Data[num_of_users].username);
        window.clear();
        window.draw(background);
        window.draw(esc);
        window.draw(ESC);
        window.draw(shape);
        window.draw(backgroundd);
        window.draw(ok);
        if (Data[num_of_users].username.size() >= 1 && !Repeated_name) window.draw(background_enter);
        else if (Repeated_name) window.draw(error_message);
        window.draw(Playername);
        window.draw(slide);
        window.display();
        deltaTime = c.getElapsedTime().asSeconds();
    }

}

void loadGame(RenderWindow& window, Sprite& background, Music& bg_music) {

    RectangleShape shape(Vector2f(1920, 1080));
    shape.setFillColor(Color(0, 0, 0, 122));

    Texture saved_users_t;
    saved_users_t.loadFromFile("./assets/textures/saved_users.png");
    Sprite saved_users;
    saved_users.setTexture(saved_users_t);

    Font font;
    font.loadFromFile("./assets/font/HeadlinerNo.45 DEMO.ttf");

    Texture esc_t;
    esc_t.loadFromFile("./assets/textures/esc.png");
    Sprite esc;
    esc.setTexture(esc_t);
    Text ESC;
    ESC.setFont(font);
    ESC.setCharacterSize(37);
    ESC.setString("esc");
    ESC.setPosition(25, 6);

    Music enter;
    enter.openFromFile("./assets/sounds/enter.mp3");
    enter.setVolume(50);
    Music click;
    click.openFromFile("./assets/sounds/click.mp3");
    click.setVolume(50);

    Text text[MAX_PLAYERS];
    Text text_lvl[MAX_PLAYERS];

    for (int i = 0; i < MAX_PLAYERS; i++) {
        text[i].setFillColor(Color::White);
        text[i].setOutlineColor(Color::Black);
        text[i].setOutlineThickness(0.2);
        text[i].setCharacterSize(50);
        text[i].setFont(font);

        text_lvl[i].setFillColor(Color::White);
        text_lvl[i].setOutlineColor(Color::Black);
        text_lvl[i].setOutlineThickness(0.23);
        text_lvl[i].setCharacterSize(50);
        text_lvl[i].setFont(font);
    }

    for (int i = 0; i < num_of_users; i++) {
        text[i].setString(Data[i].username);
        text[i].setPosition(118, 510 + (100 * i));
        text_lvl[i].setString("level " + to_string(Data[i].level));
        text_lvl[i].setPosition(118 + 400, 510 + (100 * i));
    }

    Clock c;
    float deltaTime = 0, timer = 0;
    int B = 0, B2 = 0;
    bool isMouse = false;
    while (window.isOpen()) {
        c.restart();
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            //mouse
            RectangleShape mouse(Vector2f(15, 20));
            Vector2i mousePosition = Mouse::getPosition(window);
            Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);
            mouse.setPosition(worldMousePosition);

            replay_lvl1 = false;
            replay_lvl2 = false;
            replay_lvl3 = false;
            replay_lvl4 = false;

            for (int i = 0; i < num_of_users; ++i) {
                if (mouse.getGlobalBounds().intersects(text[i].getGlobalBounds())) {
                    if (Mouse::isButtonPressed(Mouse::Left)) {
                        enter.play();
                        bg_music.stop();
                        PageNumber = -3;
                        loading(window, Data[name_selected].level);
                        if (sound) bg_music.play();
                    }
                    name_selected = i;
                    isMouse = true;
                    break;
                }
                else {
                    isMouse = false;
                }
            }
            if (event.type == Event::KeyPressed && !isMouse) {
                if (event.key.code == Keyboard::Up) {
                    click.play();
                    if (name_selected == 0) { name_selected = num_of_users - 1; }
                    else { name_selected--; }
                }

                if (event.key.code == Keyboard::Down) {
                    click.play();
                    if (name_selected == num_of_users - 1) { name_selected = 0; }
                    else { name_selected++; }
                }
                if (event.key.code == Keyboard::Enter) {
                    enter.play();
                    bg_music.stop();

                    PageNumber = -3;
                    loading(window, Data[name_selected].level);

                    if (sound) bg_music.play();
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                enter.play();
                sleep(milliseconds(150));
                PageNumber = 1000;
                return;
            }
            if (mouse.getGlobalBounds().intersects(ESC.getGlobalBounds())) {
                ESC.setFillColor(Color{ 115,158,170 });
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    enter.play();
                    sleep(milliseconds(150));
                    PageNumber = 1000;
                    return;
                }
            }
            else {
                ESC.setFillColor(Color::White);
            }
        }
        if (timer < 0) {
            background.setTextureRect(IntRect(B * 1920, B2 * 1080, 1920, 1080));
            B++;
            B %= 9;
            if (B == 8) {
                B = 0;
                B2++;
            }
            if (B2 == 7) {
                B2 = 0;
            }
            timer = 0.066;
        }
        else {
            timer -= deltaTime;
        }
        for (int i = 0; i < num_of_users; ++i) {
            if (i == name_selected) {
                text[i].setFillColor(Color{ 115,158,170 });
                text_lvl[i].setFillColor(Color{ 115,158,170 });
            }
            else {
                text[i].setFillColor(Color::White);
                text_lvl[i].setFillColor(Color::White);
            }
        }

        window.clear();
        window.draw(background);
        window.draw(esc);
        window.draw(ESC);
        window.draw(shape);
        window.draw(saved_users);
        for (int i = 0; i < num_of_users; i++) {
            window.draw(text[i]);
            window.draw(text_lvl[i]);
        }
        window.display();
        deltaTime = c.getElapsedTime().asSeconds();
    }
}

void options(RenderWindow& window, Sprite& background, Music& bg_music) {
    RectangleShape shape(Vector2f(1920, 1080));
    shape.setFillColor(Color(0, 0, 0, 122));

    Font font;
    font.loadFromFile("./assets/font/HeadlinerNo.45 DEMO.ttf");


    Texture backgroundd_T;
    backgroundd_T.loadFromFile("./assets/textures/Untitled968.png");
    Sprite backgroundd;
    backgroundd.setTexture(backgroundd_T);

    Texture on_off_T;
    on_off_T.loadFromFile("./assets/textures/sound1.png");
    Sprite on_off;
    on_off.setTexture(on_off_T);
    on_off.setScale(0.2, 0.2);
    on_off.setPosition(610, 365);

    Texture MONT;
    MONT.loadFromFile("./assets/textures/MON.png");
    Sprite MON;
    MON.setTexture(MONT);
    Texture MOFFT;
    MOFFT.loadFromFile("./assets/textures/MOFF.png");
    Sprite MOFF;
    MOFF.setTexture(MOFFT);
    Sprite MON2;
    MON2.setTexture(MONT);
    MON2.setPosition(0, 254);
    Sprite MOFF2;
    MOFF2.setTexture(MOFFT);
    MOFF2.setPosition(0, 254);

    Texture MbarT;
    MbarT.loadFromFile("./assets/textures/Mbar1.png");
    Texture MbarT2;
    MbarT2.loadFromFile("./assets/textures/Mbar2.png");
    Sprite Mbar;
    Mbar.setTexture(MbarT);
    Sprite Mbar2;
    Mbar2.setTexture(MbarT2);
    Mbar2.setPosition(695, 529.2);
    Mbar2.setColor(Color{ 115,158,170 });
    Mbar2.setScale(volume, 1);
    Text perc;
    perc.setFont(font);
    perc.setFillColor(Color::White);
    perc.setCharacterSize(37);
    String s;
    perc.setPosition(701, 524);

    Sprite Mbar3;
    Mbar3.setTexture(MbarT2);
    Mbar3.setPosition(680, 529.2);
    Mbar3.setScale(1.3, 1);
    Music enter;
    enter.openFromFile("./assets/sounds/enter.mp3");
    enter.setVolume(50);

    Text on;
    on.setFont(font);
    on.setFillColor(Color::White);
    on.setCharacterSize(37);
    on.setString("on");
    on.setPosition(200, 524);
    Text off;
    off.setFont(font);
    off.setFillColor(Color::White);
    off.setCharacterSize(37);
    off.setString("off");
    off.setPosition(465, 524);

    Text easy;
    easy.setFont(font);
    easy.setFillColor(Color::White);
    easy.setCharacterSize(37);
    easy.setString("easy");
    easy.setPosition(191, 778);
    Text hardT;
    hardT.setFont(font);
    hardT.setFillColor(Color::White);
    hardT.setCharacterSize(37);
    hardT.setString("hard");
    hardT.setPosition(456, 778);

    Texture esc_t;
    esc_t.loadFromFile("./assets/textures/esc.png");
    Sprite esc;
    esc.setTexture(esc_t);
    Text ESC;
    ESC.setFont(font);
    ESC.setCharacterSize(37);
    ESC.setString("esc");
    ESC.setPosition(25, 6);

    RectangleShape mouse(Vector2f(15, 20));

    Clock c;
    float deltaTime = 0, timer = 0;
    int B = 0, B2 = 0;
    while (window.isOpen()) {
        if (PageNumber != -2) break;
        bg_music.setVolume(25 * volume);
        c.restart();
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            //mouse
            if (mouse.getGlobalBounds().intersects(on.getGlobalBounds())) {
                on.setFillColor(Color(28, 136, 17));
                if (Mouse::isButtonPressed(Mouse::Left) && !sound) {
                    enter.play();
                    sound = true;
                    if ((bg_music.getStatus() == Music::Paused)) {
                        bg_music.play();
                    }
                }
            }
            else {
                on.setFillColor(Color::White);
            }
            if (mouse.getGlobalBounds().intersects(off.getGlobalBounds())) {
                off.setFillColor(Color(136, 17, 17));
                if (Mouse::isButtonPressed(Mouse::Left) && sound) {
                    enter.play();
                    sound = false;
                    if ((bg_music.getStatus() == Music::Playing)) {
                        bg_music.pause();
                    }
                }
            }
            else {
                off.setFillColor(Color::White);
            }

            if (mouse.getGlobalBounds().intersects(easy.getGlobalBounds())) {
                easy.setFillColor(Color(28, 136, 17));
                if (Mouse::isButtonPressed(Mouse::Left) && hard) {
                    enter.play();
                    hard = false;
                }
            }
            else {
                easy.setFillColor(Color::White);
            }
            if (mouse.getGlobalBounds().intersects(hardT.getGlobalBounds())) {
                hardT.setFillColor(Color(136, 17, 17));
                if (Mouse::isButtonPressed(Mouse::Left) && !hard) {
                    enter.play();
                    hard = true;
                }
            }
            else {
                hardT.setFillColor(Color::White);
            }

            if (mouse.getGlobalBounds().intersects(ESC.getGlobalBounds())) {
                ESC.setFillColor(Color{ 115,158,170 });
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    enter.play();
                    sleep(milliseconds(150));
                    PageNumber = 1000;
                    return;
                }
            }
            else {
                ESC.setFillColor(Color::White);
            }
        }

        if (mouse.getGlobalBounds().intersects(Mbar3.getGlobalBounds()) && sound) {
            if (Mouse::isButtonPressed(Mouse::Left)) {
                Mbar2.setScale((mouse.getPosition().x - 695 + 14.3) / 412, 1);
            }
            if (Mbar2.getScale().x > 1) Mbar2.setScale(1, 1);
            else if (Mbar2.getScale().x < 0) Mbar2.setScale(0, 1);
        }

        //mouse
        Vector2i mousePosition = Mouse::getPosition(window);
        Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);
        mouse.setPosition(worldMousePosition);

        //bg
        if (timer < 0) {
            background.setTextureRect(IntRect(B * 1920, B2 * 1080, 1920, 1080));
            B++;
            B %= 9;
            if (B == 8) {
                B = 0;
                B2++;
            }
            if (B2 == 7) {
                B2 = 0;
            }
            timer = 0.066;
        }
        else {
            timer -= deltaTime;
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            enter.play();
            sleep(milliseconds(150));
            PageNumber = 1000;
            return;
        }
        s = to_string(int(Mbar2.getScale().x * 100));
        perc.setString(s);

        volume = float(Mbar2.getScale().x);

        if (sound) on.setFillColor(Color(28, 136, 17));
        else off.setFillColor(Color(136, 17, 17));
        if (hard) hardT.setFillColor(Color(136, 17, 17));
        else easy.setFillColor(Color(28, 136, 17));
        window.clear();
        window.draw(background);
        window.draw(esc);
        window.draw(ESC);
        window.draw(shape);
        window.draw(backgroundd);
        window.draw(on);
        window.draw(off);
        window.draw(easy);
        window.draw(hardT);
        window.draw(on_off);
        if (sound) {
            on_off.setTextureRect(IntRect(0, 0, 512, 512));
            window.draw(Mbar);
            window.draw(Mbar2);
            window.draw(MON);
            window.draw(perc);
        }
        else {
            on_off.setTextureRect(IntRect(512, 0, 512, 512));
            window.draw(MOFF);
        }
        if (hard) window.draw(MOFF2);
        else window.draw(MON2);
        window.display();
        deltaTime = c.getElapsedTime().asSeconds();
    }
}

void controls(RenderWindow& window, Sprite& background) {
    RectangleShape shape(Vector2f(1920, 1080));
    shape.setFillColor(Color(0, 0, 0, 122));

    Texture controls_t;
    controls_t.loadFromFile("./assets/textures/controls.png");
    Sprite controls;
    controls.setTexture(controls_t);

    Music enter;
    enter.openFromFile("./assets/sounds/enter.mp3");
    enter.setVolume(50);

    Font font;
    font.loadFromFile("./assets/font/HeadlinerNo.45 DEMO.ttf");
    Texture esc_t;
    esc_t.loadFromFile("./assets/textures/esc.png");
    Sprite esc;
    esc.setTexture(esc_t);
    Text ESC;
    ESC.setFont(font);
    ESC.setCharacterSize(37);
    ESC.setString("esc");
    ESC.setPosition(25, 6);
    RectangleShape mouse(Vector2f(15, 20));
    Clock c;
    float deltaTime = 0, timer = 0;
    int B = 0, B2 = 0;
    while (window.isOpen()) {
        if (PageNumber != -8) break;
        c.restart();
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            //mouse
            if (mouse.getGlobalBounds().intersects(ESC.getGlobalBounds())) {
                ESC.setFillColor(Color{ 115,158,170 });
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    enter.play();
                    sleep(milliseconds(150));
                    PageNumber = 1000;
                    return;
                }
            }
            else {
                ESC.setFillColor(Color::White);
            }
        }

        //mouse
        Vector2i mousePosition = Mouse::getPosition(window);
        Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);
        mouse.setPosition(worldMousePosition);

        //bg
        if (timer < 0) {
            background.setTextureRect(IntRect(B * 1920, B2 * 1080, 1920, 1080));
            B++;
            B %= 9;
            if (B == 8) {
                B = 0;
                B2++;
            }
            if (B2 == 7) {
                B2 = 0;
            }
            timer = 0.066;
        }
        else {
            timer -= deltaTime;
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            enter.play();
            sleep(milliseconds(150));
            PageNumber = 1000;
            return;
        }

        window.clear();
        window.draw(background);
        window.draw(esc);
        window.draw(ESC);
        window.draw(shape);
        window.draw(controls);
        window.display();
        deltaTime = c.getElapsedTime().asSeconds();
    }
}

void loading(RenderWindow& window, int windowNum) {

    RectangleShape shape(Vector2f(1920, 1080));
    shape.setFillColor(Color(0, 0, 0, 190));

    Texture bar_T;
    bar_T.loadFromFile("./assets/textures/LoadingBar.png");
    Sprite LoadingBar;
    LoadingBar.setTexture(bar_T);

    Texture slot_t;//113.375
    //*0.094479166667
    slot_t.loadFromFile("./assets/textures/slot 0.png");
    Sprite slot;
    slot.setTexture(slot_t);
    slot.setPosition(slot.getPosition().x + 38, slot.getPosition().y + 10);
    slot.setScale(0.266, 0.266);

    //menu background
    Texture background_T;
    background_T.loadFromFile("./assets/textures/Untitled965 (01).png");
    Sprite background;
    background.setTexture(background_T);

    Texture bar_TT;
    bar_TT.loadFromFile("./assets/textures/bar.png");
    Sprite Bar;
    Bar.setTexture(bar_TT);
    Bar.setPosition(106, 925);

    Font font;
    font.loadFromFile("./assets/font/HeadlinerNo.45 DEMO.ttf");
    string s = "Loading";
    Text t;
    t.setFont(font);
    t.setCharacterSize(36);
    t.setPosition(118, 870);

    Clock c;
    float deltaTime = 0, timer = 0, timer2 = 0;
    int B = 0, B2 = 0;
    while (window.isOpen()) {
        if (PageNumber != -3) break;
        c.restart();
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        //bar
        if (Bar.getScale().x < 106.6) {
            for (int i = 0; i < 50; i++)
                Bar.scale(1.00054, 1);
        }
        else {
            PageNumber = windowNum;
        }

        //bg
        if (timer < 0) {
            background.setTextureRect(IntRect(B * 1920, B2 * 1080, 1920, 1080));
            B++;
            B %= 9;
            if (B == 8) {
                B = 0;
                B2++;
            }
            if (B2 == 7) {
                B2 = 0;
            }
            timer = 0.066;
        }
        else {
            timer -= deltaTime;
        }

        //Loading...
        if (timer2 < 0) {
            s = s + '.';
            timer2 = 0.23;
        }
        else {
            timer2 -= deltaTime;
        }
        if (s.size() > 11) {
            s = "Loading";
        }
        t.setString(s);


        if (PageNumber == 0) {
            intro(window);
        }
        else if (PageNumber == 1) {
            gameWindow1(window);
        }
        else if (PageNumber == 2) {
            gameWindow2(window);
        }
        else if (PageNumber == 3) {
            gameWindow2(window);
        }
        else if (PageNumber == 4) {
            gameWindow4(window, slot);
        }
        else if (PageNumber == 5) {
            gameWindow5(window);
        }
        else if (PageNumber == 1000) {
            return;
        }
        window.clear();
        window.draw(background);
        window.draw(shape);
        window.draw(Bar);
        window.draw(LoadingBar);
        window.draw(t);
        window.display();
        deltaTime = c.getElapsedTime().asSeconds();
    }
}

void intro(RenderWindow& window) {
    RectangleShape black(Vector2f(1920, 1080));

    RectangleShape shape1(Vector2f(1920, 100)), shape2(Vector2f(1920, 100));
    shape1.setFillColor(Color::Black);          shape2.setFillColor(Color::Black);
    shape2.setPosition(0, 1080 - 100);

    Texture L_bg;
    L_bg.loadFromFile("./assets/textures/1.jpg");
    Texture N_bg;
    N_bg.loadFromFile("./assets/textures/2.jpg");
    Texture N_bg_t;
    N_bg_t.loadFromFile("./assets/textures/3.png");

    Sprite Light_background;
    Light_background.setTexture(L_bg);
    Light_background.setScale(1.53, 1);
    Sprite Night_background;
    Night_background.setTexture(N_bg);
    Night_background.setScale(1.53, 1);
    Night_background.setPosition(0, -429);
    Sprite Night_background_t;
    Night_background_t.setTexture(N_bg_t);
    Night_background_t.setScale(1.53, 1);
    Night_background_t.setPosition(0, -429);

    Texture proT;
    proT.loadFromFile("./assets/textures/professor_s.png");

    Sprite pro;
    pro.setTexture(proT);
    pro.setPosition(1900, 640);
    pro.setScale(3, 3);

    Sprite pro2;
    pro2.setTexture(proT);
    pro2.setPosition(-64, 752);
    pro2.setTextureRect(IntRect(0 * 64, 64, 64, 64));

    Texture playerT;
    playerT.loadFromFile("./assets/textures/Hero_P19.png");

    Sprite player;
    player.setTexture(playerT);
    player.setPosition(2100, 787);
    player.setScale(3.9, 3.9);

    Sprite player2;
    player2.setTexture(playerT);
    player2.setPosition(2100, 758);
    player2.setScale(3.9, 3.9);
    player2.setTextureRect(IntRect(0 * 64, 7 * 64, 64, 64));

    Music start_music;
    start_music.openFromFile("./assets/sounds/music0.mp3");
    start_music.setVolume(70 * volume);
    Music start;
    start.openFromFile("./assets/sounds/story0.mp3");
    Music bgMusic;
    bgMusic.openFromFile("./assets/sounds/bg_music.mp3");
    bgMusic.setVolume(70 * volume);

    Music cars;
    cars.openFromFile("./assets/sounds/cars-test.mp3");
    cars.setVolume(80);

    Music enter;
    enter.openFromFile("./assets/sounds/enter.mp3");
    enter.setVolume(50);

    Music voice;
    voice.openFromFile("./assets/sounds/gdootest.mp3");
    Music voice_sp;
    voice_sp.openFromFile("./assets/sounds/Untitledspainnn.mp3");

    Music stefan;
    stefan.openFromFile("./assets/sounds/stefan.mp3");
    Music no;
    no.openFromFile("./assets/sounds/no.mp3");

    Music foot;
    foot.openFromFile("./assets/sounds/foot.mp3");
    foot.setVolume(39);
    Music foot6;
    foot6.openFromFile("./assets/sounds/foot.mp3");
    foot6.setVolume(39);
    Music foot2;
    foot2.openFromFile("./assets/sounds/foot2.mp3");
    foot2.setVolume(39);
    Music foot3;
    foot3.openFromFile("./assets/sounds/foot3.mp3");
    foot3.setVolume(34);

    Vector2f pos(0, 0), pos2(0, 0);

    Clock c;
    float deltaTime = 0, timer1 = 0, time06 = 0, timer2 = 0, delay = 0.08, delay2 = 0.098, delay06 = 1, L_pos = -2;
    int i = 0, j = 0, k = 0, k2 = 0, i2 = 0, speed = 100, speed2 = 116, counter = 0, obs = 255, timer = 0, join = 0;
    String text;

    //tour texture
    Texture tour_text[4];
    tour_text[0].loadFromFile("./assets/textures/r2.png");
    tour_text[1].loadFromFile("./assets/textures/r3.png");
    tour_text[2].loadFromFile("./assets/textures/r4.png");
    Sprite tour[4];
    tour[0].setPosition(2160, 820);
    tour[1].setPosition(2080, 820);
    tour[2].setPosition(2140, 900);

    Font font;
    font.loadFromFile("./assets/font/trebuc.ttf");
    Text syntax;
    syntax.setFont(font);
    syntax.setCharacterSize(25);
    bool Light = true;
    bool feet = false;
    Text skip;
    skip.setFont(font);
    skip.setCharacterSize(25);
    skip.setString("Press [Enter] to skip");
    skip.setPosition(30, 30);

    start.play();
    if (sound) {
        start_music.play();
    }
    while (window.isOpen()) {
        c.restart();
        if (PageNumber != 0) { break; }
        if (Keyboard::isKeyPressed(Keyboard::Enter)) {
            foot3.stop();
            foot2.stop();
            foot6.stop();
            foot.stop();
            no.stop();
            stefan.stop();
            voice_sp.stop();
            voice.stop();
            cars.stop();
            bgMusic.stop();
            start_music.stop();
            start.stop();
            enter.play();
            PageNumber = -3;
            loading(window, 1);
            break;
        }
        else if (player2.getPosition().y < -777) {
            foot3.stop();
            foot2.stop();
            foot6.stop();
            foot.stop();
            no.stop();
            stefan.stop();
            voice_sp.stop();
            voice.stop();
            cars.stop();
            start_music.stop();
            start.stop();
            PageNumber = 1;
            gameWindow1(window);
            break;
        }
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

        }
        black.setFillColor(Color(0, 0, 0, obs)); //fade
        Light_background.setPosition(0, L_pos); //moving down

        if (timer > 15000) {
            if (counter <= 53) {
                if (!join) {
                    if (sound) {
                        cars.play();
                        foot.play();
                        foot2.play();
                    }
                    voice.play();
                    voice_sp.play();
                    join = true;
                }
                /*     #####################   intro here   #####################     */
                if (pro.getPosition().x > 1200) {
                    if (timer1 < 0) {
                        i++;
                        i = i % 9;
                        pro.setTextureRect(IntRect(i * 64, 64, 64, 64));
                        timer1 = delay;
                    }
                    else {
                        timer1 -= deltaTime;
                    }
                    pos.x = -speed * deltaTime;
                }
                else if (counter >= 37) {
                    if (counter == 37) foot.play();
                    if (timer1 < 0) {
                        i++;
                        i = i % 9;
                        pro.setTextureRect(IntRect(i * 64, 64, 64, 64));
                        timer1 = delay;
                    }
                    else {
                        timer1 -= deltaTime;
                    }
                    pos.x = -speed * deltaTime;
                }
                else {
                    pro.setTextureRect(IntRect(0, 64, 64, 64));
                    pos.x = 0;
                    foot.pause();
                }

                if (pro.getPosition().x < 1800) {

                    if (player.getPosition().x > 1060) {
                        if (timer2 < 0) {
                            for (int i = 0; i < 3; i++)
                                tour[i].setOrigin(tour[i].getTextureRect().width / 2, tour[i].getTextureRect().height / 2);
                            j++;
                            j = j % 8;
                            player.setTextureRect(IntRect(j * 64, 7 * 64, 64, 64));

                            timer2 = delay2;
                            for (int i = 0; i < 3; i++)
                            {
                                tour[i].setTexture(tour_text[i]);
                            }

                            tour[0].setTextureRect(IntRect(k * 30, 0, 30, 47));
                            tour[1].setTextureRect(IntRect(k * 32, 0, 32, 50));
                            tour[2].setTextureRect(IntRect(k * 31, 0, 31, 45));

                            k++;
                            k %= 4;
                            for (int i = 0; i < 3; i++)
                                tour[i].setScale(-3.3, 3.3);
                        }
                        else {
                            timer2 -= deltaTime;
                        }
                        pos2.x = -speed2 * deltaTime;
                        for (int i = 0; i < 3; i++)
                        {
                            tour[i].move(-speed2 * deltaTime, 0);
                        }
                    }
                    else {
                        if (voice.getStatus() != voice.Stopped)
                        {
                            player.setTextureRect(IntRect(0 * 64, 1 * 64, 64, 64));
                            pos2.x = 0;
                            for (int i = 0; i < 3; i++)
                            {
                                tour[i].move(0, 0);
                            }
                            for (int i = 0; i < 3; i++)
                            {
                                tour[i].setScale(3.3, 3.3);
                            }
                            foot2.pause();
                        }
                        else
                        {
                            if (timer2 < 0) {
                                for (int i = 0; i < 3; i++)
                                    tour[i].setOrigin(tour[i].getTextureRect().width / 2, tour[i].getTextureRect().height / 2);
                                j++;
                                j = j % 8;
                                player.setTextureRect(IntRect(j * 64, 7 * 64, 64, 64));

                                timer2 = delay2;
                                for (int i = 0; i < 3; i++)
                                {
                                    tour[i].setTexture(tour_text[i]);
                                }

                                tour[0].setTextureRect(IntRect(k * 30, 0, 30, 47));
                                tour[1].setTextureRect(IntRect(k * 32, 0, 32, 50));
                                tour[2].setTextureRect(IntRect(k * 31, 0, 31, 45));

                                k++;
                                k %= 4;
                                for (int i = 0; i < 3; i++)
                                    tour[i].setScale(-3.3, 3.3);
                            }
                            else {
                                timer2 -= deltaTime;
                            }
                            pos2.x = -speed2 * deltaTime;
                            for (int i = 0; i < 3; i++)
                            {
                                tour[i].move(-speed2 * deltaTime, 0);
                            }


                        }
                    }
                }
                else {
                    player.setTextureRect(IntRect(0, 120, 90, 120));
                    pos2.x = 0;
                }
                if (counter == 35) foot3.play();
                pro.move(pos);
                player.move(pos2);

                //////////////////////  syntax 2  //////////////////////
                if (counter > 4 && counter <= 7)
                    text = "(Dr. Mohamed) Opener, OF All-Knowing, OUR Sustainer, OF Generous -arabic";
                else if (counter > 7 && counter <= 10)
                    text = "and now the final stop, make sure not to touch anything";
                else if (counter > 10 && counter <= 13)
                    text = "the legend says that this pyramid is cursed";
                else if (counter > 13 && counter <= 15)
                    text = "and the last one who went there never came back";
                else if (counter > 15 && counter <= 17)
                    text = "bullshit -arabic";
                else if (counter > 17 && counter <= 19)
                    text = "so be careful guys";
                else if (counter > 19 && counter <= 20)
                    text = "(Stefan) Excuse me doc";
                else if (counter > 20 && counter <= 21)
                    text = "(Dr. Mohamed) Yes?";
                else if (counter > 21 && counter <= 23)
                    text = "(Stefan) Can we get more information about the curse?";
                else if (counter > 23 && counter <= 25)
                    text = "(Dr. Mohamed) I'm afraid not,";
                else if (counter > 25 && counter <= 27)
                    text = "the government closed the entries years ago";
                else if (counter > 27 && counter <= 30)
                    text = "and all the books about the curse were burned in 1864 so,";
                else if (counter > 30 && counter <= 32)
                    text = "I can't help you";
                else if (counter > 32 && counter <= 34)
                    text = "anyway, I think it's time to leave";
                else if (counter > 34 && counter <= 36)
                    text = "(Lydia) Finally! I was so hungry";
                else if (counter > 36 && counter <= 38)
                    text = "(Stefan) Yeah, me too";
                else
                    text = "";

                syntax.setString(text);
                syntax.setPosition(1920 / 2, 1025);
                syntax.setOrigin(syntax.getGlobalBounds().width / 2.1, syntax.getGlobalBounds().height / 1.68);
            }
            if (counter == 42) {
                stefan.play();
                no.play();
            }
            if (counter > 53) {

                //////////////////////  syntax 3  //////////////////////
                if (counter > 62 && counter <= 63)
                    text = "(Stefan) Finally i'm alone,";
                else if (counter > 63 && counter <= 66)
                    text = "now i can know all about that stupid curse";
                else if (counter > 68 && counter <= 69)
                    text = "(Dr. Mohamed) Stefan why are you here?";
                else if (counter > 69 && counter <= 71)
                    text = "you can't go inside";
                else if (counter > 71 && counter <= 72)
                    text = "(Stefan) Why not?";
                else if (counter > 72 && counter <= 74)
                    text = "(Dr. Mohamed) Because you'll be cursed, come here now";
                else if (counter > 74 && counter <= 76)
                    text = "(Stefan) Sorry doc, too late";
                else if (counter > 76 && counter <= 78)
                    text = "(Dr. Mohamed) NOO!";
                else
                    text = "";
                syntax.setString(text);
                syntax.setPosition(1920 / 2, 1025);
                syntax.setOrigin(syntax.getGlobalBounds().width / 2.1, syntax.getGlobalBounds().height / 1.68);

                if (player2.getPosition().x > 861) {
                    if (join == -1) {
                        foot6.play();
                        join = -2;
                    }
                    if (timer2 < 0) {
                        player2.setTextureRect(IntRect(k2 * 64, 7 * 64, 64, 64));
                        k2++;
                        k2 %= 8;
                        timer2 = delay2;
                    }
                    else {
                        timer2 -= deltaTime;
                    }
                    player2.move(-speed * deltaTime, 0);
                }
                else if (player2.getPosition().x <= 861 && pro2.getPosition().x < 820) {
                    if (join == -2) {
                        foot.play();
                        join = -3;
                    }
                    if (timer1 < 0) {
                        i2++;
                        i2 = i2 % 9;
                        pro2.setScale(-3, 3);
                        pro2.setTextureRect(IntRect(i2 * 64, 64, 64, 64));
                        timer1 = delay;
                    }
                    else {
                        timer1 -= deltaTime;
                    }
                    pro2.move((speed - 40) * deltaTime, 0);
                }
                if (no.getStatus() == no.Stopped) { foot.stop(); }
                if (stefan.getStatus() == stefan.Stopped) {
                    if (join == -3) {
                        foot6.play();
                        join = -4;
                    }
                    if (player2.getPosition().y > 450) {
                        if (timer2 < 0) {
                            player2.setTextureRect(IntRect(k2 * 64, 10 * 64, 64, 64));
                            k2++;
                            k2 %= 8;
                            timer2 = delay2;
                        }
                        else {
                            timer2 -= deltaTime;
                        }
                        player2.move(0, -speed2 * deltaTime);
                    }
                    else if (Night_background.getPosition().y < 99) {
                        if (timer2 < 0) {
                            player2.setTextureRect(IntRect(k2 * 64, 10 * 64, 64, 64));
                            k2++;
                            k2 %= 8;
                            timer2 = delay2;
                        }
                        else {
                            timer2 -= deltaTime;
                        }
                        Night_background.move(0, speed2 * deltaTime);
                        Night_background_t.move(0, speed2 * deltaTime);
                        pro2.move(0, speed2 * deltaTime);
                    }
                    else {
                        if (timer2 < 0) {
                            player2.setTextureRect(IntRect(k2 * 64, 10 * 64, 64, 64));
                            k2++;
                            k2 %= 8;
                            timer2 = delay2;
                        }
                        else {
                            timer2 -= deltaTime;
                        }
                        player2.move(0, -speed2 * deltaTime);
                    }
                }
                else {
                    if (player2.getPosition().x <= 861 && player2.getPosition().y == 758) {
                        foot6.stop();
                        if (timer2 < 0) {
                            player2.setTextureRect(IntRect(k2 * 64, 3 * 64, 64, 64));
                            k2++;
                            k2 %= 8;
                            timer2 = delay2;
                        }
                        else {
                            timer2 -= deltaTime;
                        }
                    }
                }
            }  //////////////////

            window.clear();
            if (counter >= 53 && obs > 0 && counter < 70) {
                Light = false;
                sleep(milliseconds(20));
                obs--;
            }
            if (Light) window.draw(Light_background);
            else       window.draw(Night_background);
            window.draw(pro);
            for (int i = 0; i < 3; i++)
                window.draw(tour[i]);
            window.draw(player);
            window.draw(player2);
            window.draw(pro2);
            window.draw(Night_background_t);
            window.draw(shape1);
            window.draw(shape2);
            window.draw(syntax);
            window.draw(skip);
            window.draw(black);
            window.display();

            if (player2.getPosition().y < 0 && obs < 255 && counter > 53) {
                sleep(milliseconds(20));
                obs++;
            }
            if (counter >= 46 && obs < 255 && counter < 53) {
                if (join == 1 && sound) {
                    join = -1;
                    bgMusic.play();
                    foot.stop(); foot2.stop(); foot3.stop();
                }
                sleep(milliseconds(20));
                obs++;
            }

            if (time06 < 0) {
                counter++;
                time06 = delay06;
            }
            else {
                time06 -= deltaTime;
            }
        }

        if (timer <= 15000) {
            if (obs > 0) {
                sleep(milliseconds(45));
                obs--;
                if (L_pos > -429) {
                    L_pos -= 1.7;
                }
            }
            timer += 32;

            //////////////////////  syntax 1  //////////////////////
            if (timer > 0 && timer <= 2500)
                text = "The hole thing started with a trip to the pyramids by the school";
            else if (timer > 2500 && timer <= 5000)
                text = "this was Stefan's first time seeing the pyramids";
            else if (timer > 5000 && timer <= 7500)
                text = "but what he didn't know is that he should've stayed home";
            else
                text = "";

            syntax.setString(text);
            syntax.setPosition(1920 / 2, 1025);
            syntax.setOrigin(syntax.getGlobalBounds().width / 2.1, syntax.getGlobalBounds().height / 1.68);

            window.clear();
            window.draw(Light_background);
            window.draw(black);
            window.draw(shape1);
            window.draw(shape2);
            window.draw(syntax);
            window.draw(skip);
            window.display();
        }
        deltaTime = c.getElapsedTime().asSeconds();
    }
}

void gameWindow1(RenderWindow& window) {
    edit_data();
    Clock c;
    float deltaTime = 0;
    const int num_of_coins = 4, num_of_time = 3, num_of_power = 3;

    RectangleShape black_screen(Vector2f(1920, 1080));
    black_screen.setFillColor(Color(0, 0, 0, 190));

    //Music
    Music coin_is_collected;
    coin_is_collected.openFromFile("./assets/sounds/coin collected.wav");
    coin_is_collected.setVolume(30);

    Music foot_steps;
    foot_steps.openFromFile("./assets/sounds/foot.mp3");
    foot_steps.setVolume(39);
    foot_steps.setLoop(true);
    bool movingRight = 0, movingLeft = 0, movingUp = 0, movingDown = 0;

    Music boy_breathing_out;
    boy_breathing_out.openFromFile("./assets/sounds/boy_breathing_out.mp3");
    boy_breathing_out.setVolume(39);
    boy_breathing_out.setLoop(true);

    Music end_of_game;
    end_of_game.openFromFile("./assets/sounds/end_of_game (1).mp3");
    end_of_game.setVolume(39);

    Music indoor_footsteps;
    indoor_footsteps.openFromFile("./assets/sounds/steps-on-ground (1).mp3");
    indoor_footsteps.setVolume(39);

    Music gate_sound;
    gate_sound.openFromFile("./assets/sounds/gate.mp3");
    gate_sound.setVolume(17);

    Music YOU_WIN;
    YOU_WIN.openFromFile("./assets/sounds/YOU WIN 5.wav");

    Music music_bg;
    music_bg.openFromFile("./assets/sounds/bg_music_FULL.mp3");
    music_bg.setVolume(28 * volume);
    music_bg.setLoop(true);

    Music game_over_sound;
    game_over_sound.openFromFile("./assets/sounds/game_over_sound.mp3");

    Music collect_all_coins;
    collect_all_coins.openFromFile("./assets/sounds/collect_all_coins.mp3");

    //mini shader
    Texture SShader_t;
    SShader_t.loadFromFile("./assets/textures/mini_shader.png");
    Sprite SShader;
    SShader.setTexture(SShader_t);
    SShader.setOrigin(SShader.getTextureRect().width / 2, SShader.getTextureRect().height / 2);

    //background
    Texture texture_background;
    Sprite sprite_baackground;
    texture_background.loadFromFile("./assets/textures/maze_full.png");
    sprite_baackground.setTexture(texture_background);

    //PLAYER
    struct player {
        float speed = 300, timer = 0, delay = 0.067f;
        char status = 'N';
        int boy_frame = 0, boy_frame_afk = 0;
        bool afk = true;
        Texture broT;
        Sprite bro;
        int position_X = 600, position_y = 600;
    }Player_1;

    Player_1.broT.loadFromFile("./assets/textures/Hero_P1.png");
    Player_1.bro.setTexture(Player_1.broT);
    Player_1.bro.setTextureRect(IntRect(0, 0, 25, 35));
    Player_1.bro.setScale(3.5, 3.5);
    Player_1.bro.setPosition(970, 3400);
    Player_1.bro.setOrigin(Player_1.bro.getTextureRect().width / 2, Player_1.bro.getTextureRect().height / 2);

    //first position for plyer
    Texture texture_Door;
    Sprite sprite_Door;
    texture_Door.loadFromFile("./assets/textures/maze door (1).png");
    sprite_Door.setTexture(texture_Door);
    float timer_first_position = 0, delay_first_position = 1;
    int counter_first_position = 0;
    indoor_footsteps.play();

    //slots
    int countr_slots_check = 0;
    Texture texture_slots;
    Sprite sprite_slots;
    texture_slots.loadFromFile("./assets/textures/slot 0.png");
    sprite_slots.setTexture(texture_slots);
    sprite_slots.setScale(0.266, 0.266);
    sprite_slots.setPosition(sprite_slots.getPosition().x + 38, sprite_slots.getPosition().y + 10);

    //mini map
    Texture mini2;
    mini2.loadFromFile("./assets/textures/mini map0.png");
    Sprite minimap2;
    minimap2.setTexture(mini2);
    minimap2.setPosition(minimap2.getPosition().x + 1680, minimap2.getPosition().y + 10);
    minimap2.setScale(0.266, 0.266);
    Texture mini_maze_t;
    mini_maze_t.loadFromFile("./assets/textures/maze_mini0.png");
    Sprite mini_maze;
    mini_maze.setTexture(mini_maze_t);
    mini_maze.setTextureRect(IntRect(0, 302 - (543 * 0.266), 678 * 0.266, 543 * 0.266));
    mini_maze.setPosition(minimap2.getPosition().x + 42 * 0.266, minimap2.getPosition().y + 39 * 0.266);

    //coins
    bool arr_draw[4] = { 1,1,1,1 };
    int currentframe_coin = 0;
    float timer_coin = 0, delay_coin = 0.03f;
    Texture textures_coin[num_of_coins];
    Sprite sprite_coin[num_of_coins];
    for (int counter_of_loops = 0; counter_of_loops < num_of_coins; ++counter_of_loops)
    {
        textures_coin[counter_of_loops].loadFromFile("./assets/textures/coin 1 - NEW.png");
        sprite_coin[counter_of_loops].setTexture(textures_coin[counter_of_loops]);
        sprite_coin[counter_of_loops].setTextureRect(IntRect(0, 0, 210, 209));
        sprite_coin[counter_of_loops].setScale(0.2, 0.2);
    }
    sprite_coin[0].setPosition(1740, 300);
    sprite_coin[1].setPosition(3060, 1060);
    sprite_coin[2].setPosition(2140, 2720);
    sprite_coin[3].setPosition(5300, 2560);

    //timer
    struct timer_of_game {
        Texture texture_sand;
        Sprite sprite_sand;
        int currentframe_sand = 0;
        float delay_sand = 1.4, timer_sand = 0;
        int counter_time = 0;
        int  timer_of_pause = 99;
        Text timerText;
        Font timerfont;
        Clock Timerclock;
    }timer1;

    //Timer as sand clock
    timer1.texture_sand.loadFromFile("./assets/textures/timer.png");
    timer1.sprite_sand.setTextureRect(IntRect(0, 0, 490, 348));
    timer1.sprite_sand.setTexture(timer1.texture_sand);
    timer1.sprite_sand.setScale(0.266, 0.266);
    timer1.sprite_sand.setPosition((1920 / 2) - (timer1.sprite_sand.getGlobalBounds().width / 2), timer1.sprite_sand.getPosition().y + 10);

    //Timer as digits
    timer1.timerfont.loadFromFile("./assets/font/trebuc.ttf");
    timer1.timerText.setFont(timer1.timerfont);
    timer1.timerText.setCharacterSize(50);
    timer1.timerText.setFillColor(sf::Color::Black);
    timer1.timerText.setPosition((1920 / 2) - (timer1.sprite_sand.getGlobalBounds().width / 2) + 16, timer1.sprite_sand.getPosition().y + 7);

    //player health
    Texture health_T[3];
    health_T[0].loadFromFile("./assets/textures/health.png");
    health_T[1].loadFromFile("./assets/textures/health2.png");
    health_T[2].loadFromFile("./assets/textures/health3.png");
    Sprite health[3];
    health[1].setPosition(Vector2f(141, 116));
    for (int i = 0; i < 3; i++) {
        health[i].setTexture(health_T[i]);
        health[i].setPosition(health[i].getPosition().x + 38, health[i].getPosition().y + 10);
        health[i].setScale(0.266, 0.266);
    }
    Texture heart_T;
    heart_T.loadFromFile("./assets/textures/heart.png");
    Sprite heart[3];
    for (int i = 0; i < 3; i++) {
        heart[i].setTexture(heart_T);
        heart[i].setTextureRect(IntRect(0, 0, 125.1, 126));
        heart[i].setScale(0.3, 0.3);
    }
    heart[2].setPosition(48.55, 125.5);
    heart[1].setPosition(48.55 + (126 * 0.3), 125.5);
    heart[0].setPosition(48.55 + (252 * 0.3), 125.5);

    //pyramid
    Texture pyramid_T;
    pyramid_T.loadFromFile("./assets/textures/pyramid.png");
    Sprite pyramid;
    pyramid.setTexture(pyramid_T);
    pyramid.setPosition(50, 20);
    pyramid.setScale(0.22, 0.3);
    float timer7 = 0;
    int p = 0;

    //map
    View map(FloatRect(0, 2240, 1920, 1080));
    map.setCenter(Player_1.bro.getPosition().x + Player_1.bro.getLocalBounds().width / 2, Player_1.bro.getPosition().y + Player_1.bro.getLocalBounds().height / 2);

    Vector2f velocity;
    //Blocks//
    const int blocksnumbers = 51;
    RectangleShape block[blocksnumbers];

    //syntax
    Music hard_mode_lvl1;
    hard_mode_lvl1.openFromFile("./assets/sounds/hard_mode_lvl1.mp3");
    Music easy_mode_lvl1;
    easy_mode_lvl1.openFromFile("./assets/sounds/easy_mode_lvl1.mp3");
    String text;
    Text syntax;
    syntax.setFont(timer1.timerfont);
    syntax.setCharacterSize(25);
    RectangleShape shape99;
    shape99.setFillColor(Color(0, 0, 0, 140));
    float TIME_R = 0, COUNTER = 0;

    for (int i = 0; i < blocksnumbers - 2; i++) {
        block[i].setScale(4.5, 4.5);
    }

    //block1;
    block[0].setPosition(150, 0);
    block[0].setSize(Vector2f(1920, 43));

    //block2;
    block[1].setPosition(5575, 110);
    block[1].setSize(Vector2f(40, 95));

    //block2_2;
    block[48].setPosition(5575, 820);
    block[48].setSize(Vector2f(40, 500));

    //block3_1;
    block[2].setPosition(1120, 3050);
    block[2].setSize(Vector2f(1950, 60));

    //block3_2;
    block[3].setPosition(200, 3050);
    block[3].setSize(Vector2f(137, 50));

    //block4;
    block[4].setPosition(0, 0);
    block[4].setSize(Vector2f(43, 1080));

    //block11;
    block[5].setPosition(510, 2283);
    block[5].setSize(Vector2f(46, 105));

    //block12;
    block[6].setPosition(1100, 2424);
    block[6].setSize(Vector2f(37, 74));

    //block13;
    block[7].setPosition(1137, 2424);
    block[7].setSize(Vector2f(300, 34));

    //block14;
    block[8].setPosition(2340, 2424);
    block[8].setSize(Vector2f(35, 74));

    //block15;
    block[9].setPosition(2490, 2580);
    block[9].setSize(Vector2f(35, 115));

    //block16;
    block[10].setPosition(1603, 2820);
    block[10].setSize(Vector2f(35, 45));

    //block21;
    block[11].setPosition(684, 1320);
    block[11].setSize(Vector2f(35, 160));

    //block22;
    block[12].setPosition(237, 1320);
    block[12].setSize(Vector2f(130, 35));

    //block23;
    block[13].setPosition(604, 645);
    block[13].setSize(Vector2f(34, 90));

    //block24;
    block[14].setPosition(1115, 650);
    block[14].setSize(Vector2f(34, 121));

    //block25;
    block[15].setPosition(803, 320);
    block[15].setSize(Vector2f(34, 45));

    //block26;
    block[16].setPosition(480, 492);
    block[16].setSize(Vector2f(358, 35));

    //block27;
    block[17].setPosition(1937, 235);
    block[17].setSize(Vector2f(34, 70));

    //block31
    block[18].setPosition(1265, 1773);
    block[18].setSize(Vector2f(34, 72));

    //block32
    block[19].setPosition(1576, 1227);
    block[19].setSize(Vector2f(34, 130));

    //block33
    block[20].setPosition(1265, 1773);
    block[20].setSize(Vector2f(380, 34));

    //block34
    block[21].setPosition(2269, 752);
    block[21].setSize(Vector2f(34, 240));

    //block35
    block[22].setPosition(2304, 1928);
    block[22].setSize(Vector2f(34, 35));

    //block36
    block[23].setPosition(2822, 1926);
    block[23].setSize(Vector2f(34, 82));

    //block37
    block[24].setPosition(2822, 2292);
    block[24].setSize(Vector2f(110, 32));

    //block38
    block[25].setPosition(3270, 2292);
    block[25].setSize(Vector2f(33, 155));

    //block39
    block[26].setPosition(1812, 857);
    block[26].setSize(Vector2f(58, 33));

    //block41
    block[27].setPosition(2662, 240);
    block[27].setSize(Vector2f(33, 230));

    //block42
    block[28].setPosition(2627, 1260);
    block[28].setSize(Vector2f(264, 33));

    //block43
    block[29].setPosition(3354, 714);
    block[29].setSize(Vector2f(33, 120));

    //block44
    block[30].setPosition(2627, 1260);
    block[30].setSize(Vector2f(264, 33));

    //block45
    block[31].setPosition(2627, 1411);
    block[31].setSize(Vector2f(33, 23));

    //block46
    block[32].setPosition(3665, 1413);
    block[32].setSize(Vector2f(33, 38));

    //block47
    block[33].setPosition(3728, 473);
    block[33].setSize(Vector2f(58, 33));

    //block48
    block[34].setPosition(3665, 1610);
    block[34].setSize(Vector2f(58, 33));

    //block49
    block[35].setPosition(3270, 1791);
    block[35].setSize(Vector2f(33, 39));

    //block51
    block[36].setPosition(3872, 2658);
    block[36].setSize(Vector2f(33, 75));

    //block52
    block[37].setPosition(3663, 2006);
    block[37].setSize(Vector2f(33, 85));

    //block53
    block[38].setPosition(3816, 2128);
    block[38].setSize(Vector2f(125, 32));

    //block54
    block[39].setPosition(4232, 240);
    block[39].setSize(Vector2f(33, 452));

    //block55
    block[40].setPosition(4384, 1591);
    block[40].setSize(Vector2f(80, 31));

    //block56
    block[41].setPosition(4717, 1314);
    block[41].setSize(Vector2f(31, 93));

    //block57
    block[42].setPosition(4717, 732);
    block[42].setSize(Vector2f(32, 75));

    //block58
    block[43].setPosition(4425, 575);
    block[43].setSize(Vector2f(58, 31));

    //block61;
    block[44].setPosition(4987, 2653);
    block[44].setSize(Vector2f(34, 77));

    //block62;
    block[45].setPosition(4987, 2075);
    block[45].setSize(Vector2f(34, 77));

    //block63;
    block[46].setPosition(4985, 1993);
    block[46].setSize(Vector2f(120, 33));

    //block64;
    block[47].setPosition(5175, 1161);
    block[47].setSize(Vector2f(78, 33));

    //block50;
    block[49].setPosition(0, 3240);
    block[49].setSize(Vector2f(1920, 100));

    //block51;
    block[50].setPosition(5760, 0);
    block[50].setSize(Vector2f(100, 1080));

    //gate to level two
    RectangleShape gate(Vector2f(50, 300));
    gate.setPosition(5755, 520);
    gate.setFillColor(Color::Blue);
    int pause_count = 0;
    bool WIN = false;
    if (!replay_lvl1) {
        if (hard) hard_mode_lvl1.play();
        else easy_mode_lvl1.play();
    }
    if (sound) music_bg.play();
    while (window.isOpen()) {
        c.restart();
        if (PageNumber != 1) break;
        sprite_Door.setPosition(0, 0);
        bool play = true;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    while (pause_count < 2) {
                        pause_count++;
                        window.draw(black_screen);
                        window.display();
                    }
                    pause_count = 0;
                    music_bg.pause();
                    if (coin_is_collected.getStatus() == Music::Playing) coin_is_collected.pause();
                    if (boy_breathing_out.getStatus() == Music::Playing) boy_breathing_out.pause();
                    if (foot_steps.getStatus() == Music::Playing) foot_steps.pause();
                    if (end_of_game.getStatus() == Music::Playing) end_of_game.pause();
                    if (indoor_footsteps.getStatus() == Music::Playing) indoor_footsteps.pause();
                    if (hard_mode_lvl1.getStatus() == Music::Playing) hard_mode_lvl1.pause();
                    if (easy_mode_lvl1.getStatus() == Music::Playing) easy_mode_lvl1.pause();
                    pause(window);
                    if (PageNumber != 1) return;
                    if (sound) music_bg.play();
                    if (coin_is_collected.getStatus() == Music::Paused) coin_is_collected.play();
                    if (boy_breathing_out.getStatus() == Music::Paused) boy_breathing_out.play();
                    if (foot_steps.getStatus() == Music::Paused) foot_steps.play();
                    if (end_of_game.getStatus() == Music::Paused) end_of_game.play();
                    if (indoor_footsteps.getStatus() == Music::Paused) indoor_footsteps.play();
                    if (hard_mode_lvl1.getStatus() == Music::Paused) hard_mode_lvl1.play();
                    if (easy_mode_lvl1.getStatus() == Music::Paused) easy_mode_lvl1.play();
                    play = false;
                    break;
                }
            }
        }
        if (play) {
            //player to level 2
            if (hard) {
                //HARD MODE
                if (Player_1.bro.getGlobalBounds().intersects(gate.getGlobalBounds()) && countr_slots_check == 4) {
                    Player_1.bro.setScale(0, 0);
                    WIN = true;
                }
                else if (Player_1.bro.getGlobalBounds().intersects(gate.getGlobalBounds()) && countr_slots_check != 4) {
                    if (collect_all_coins.getStatus() == Music::Stopped) {
                        collect_all_coins.play();
                    }
                }
            }
            else {
                //EASY MODE
                if (Player_1.bro.getGlobalBounds().intersects(gate.getGlobalBounds())) {
                    Player_1.bro.setScale(0, 0);
                    WIN = true;
                }
            }
            //game over
            if (timer1.timer_of_pause == 0) {
                music_bg.stop();
                coin_is_collected.stop();
                boy_breathing_out.stop();
                foot_steps.stop();
                boy_breathing_out.stop();
                end_of_game.stop();
                indoor_footsteps.stop();
                game_over_sound.play();
                sleep(milliseconds(200));
                PageNumber = -6;
                replay_lvl1 = true;
                gameOver(window, 1);
                if (PageNumber != 1) return;
            }

            //first position for plyer
            if (counter_first_position <= 35) {
                if (timer_first_position < 0) {
                    Player_1.bro.setTextureRect(IntRect(Player_1.boy_frame * 22, 210, 22, 38));
                    Player_1.boy_frame++;
                    Player_1.boy_frame %= 8;
                    timer_first_position = Player_1.delay;
                    counter_first_position++;
                    Player_1.bro.setPosition(Player_1.bro.getPosition().x, Player_1.bro.getPosition().y - 10);
                }
                else {
                    timer_first_position -= deltaTime;
                }
                Player_1.status = 'W';
                Player_1.afk = true;
            }
            else {
                //movemwent player (all directions)
                velocity.x = 0;
                velocity.y = 0;
                bool collision = false;

                // movement on y axis //
                if (Keyboard::isKeyPressed(Keyboard::W)) {
                    velocity.y = -Player_1.speed * deltaTime;
                    Player_1.status = 'W';
                    Player_1.afk = false;
                    map.move(0, -Player_1.speed * deltaTime);
                    if (Player_1.timer < 0) {
                        Player_1.bro.setTextureRect(IntRect(Player_1.boy_frame * 22, 210, 22, 38));
                        Player_1.boy_frame++;
                        Player_1.boy_frame %= 8;
                        Player_1.timer = Player_1.delay;
                    }
                    else { Player_1.timer -= deltaTime; }

                    //check collision before moving map

                    Player_1.bro.move(0, velocity.y);
                    for (int i = 0; i < blocksnumbers; i++) {
                        if (Player_1.bro.getGlobalBounds().intersects(block[i].getGlobalBounds()) && !collision) {
                            collision = true;
                        }
                        if (Player_1.bro.getGlobalBounds().intersects(block[i].getGlobalBounds())) {
                            Player_1.bro.move(0, -velocity.y);
                        }
                    }
                    if (!collision) {
                        Player_1.bro.move(0, -Player_1.speed * deltaTime);
                        map.move(0, -Player_1.speed * deltaTime);
                    }
                    else {
                        // Move map back when collision
                        map.move(0, Player_1.speed * deltaTime);
                    }
                }
                else if (Keyboard::isKeyPressed(Keyboard::S)) {
                    velocity.y = Player_1.speed * deltaTime;
                    if (Player_1.timer < 0) {
                        Player_1.bro.setTextureRect(IntRect(Player_1.boy_frame * 22, 140, 22, 35));
                        Player_1.boy_frame++;
                        Player_1.boy_frame %= 8;
                        Player_1.timer = Player_1.delay;
                    }
                    else { Player_1.timer -= deltaTime; }
                    Player_1.status = 'S';
                    Player_1.afk = false;
                    map.move(0, Player_1.speed * deltaTime);
                    // Check collision before moving map
                    Player_1.bro.move(0, velocity.y);
                    for (int i = 0; i < blocksnumbers; i++) {
                        if (Player_1.bro.getGlobalBounds().intersects(block[i].getGlobalBounds()) && !collision) {
                            collision = true;
                        }
                        if (Player_1.bro.getGlobalBounds().intersects(block[i].getGlobalBounds())) {
                            Player_1.bro.move(0, -velocity.y);
                        }
                    }
                    if (!collision) {
                        Player_1.bro.move(0, Player_1.speed * deltaTime);
                        map.move(0, Player_1.speed * deltaTime);
                    }
                    else {
                        map.move(0, -Player_1.speed * deltaTime);
                    }
                }

                // movement on x axis
                else if (Keyboard::isKeyPressed(Keyboard::D)) {
                    velocity.x = Player_1.speed * deltaTime;
                    if (Player_1.timer < 0) {
                        Player_1.bro.setTextureRect(IntRect(Player_1.boy_frame * 20, 175, 20, 35));
                        Player_1.boy_frame++;
                        Player_1.boy_frame %= 8;
                        Player_1.timer = Player_1.delay;
                    }
                    else { Player_1.timer -= deltaTime; }
                    Player_1.status = 'D';
                    Player_1.afk = false;
                    map.move(Player_1.speed * deltaTime, 0);
                    // Check collision before moving map
                    Player_1.bro.move(velocity.x, 0);
                    for (int i = 0; i < blocksnumbers; i++) {
                        if (Player_1.bro.getGlobalBounds().intersects(block[i].getGlobalBounds()) && !collision) {
                            collision = true;
                        }
                        if (Player_1.bro.getGlobalBounds().intersects(block[i].getGlobalBounds())) {
                            Player_1.bro.move(-velocity.x, 0);
                        }
                    }
                    if (!collision) {
                        Player_1.bro.move(Player_1.speed * deltaTime, 0);
                        map.move(Player_1.speed * deltaTime, 0);
                    }
                    else {
                        map.move(-Player_1.speed * deltaTime, 0);
                    }
                }
                else if (Keyboard::isKeyPressed(Keyboard::A)) {
                    velocity.x = -Player_1.speed * deltaTime;
                    if (Player_1.timer < 0) {
                        Player_1.bro.setTextureRect(IntRect(Player_1.boy_frame * 20, 248, 20, 35));
                        Player_1.boy_frame++;
                        Player_1.boy_frame %= 8;
                        Player_1.timer = Player_1.delay;
                    }
                    else { Player_1.timer -= deltaTime; }
                    Player_1.status = 'A';
                    Player_1.afk = false;
                    map.move(-Player_1.speed * deltaTime, 0);
                    // Check collision before moving map
                    Player_1.bro.move(velocity.x, 0);
                    for (int i = 0; i < blocksnumbers; i++) {
                        if (Player_1.bro.getGlobalBounds().intersects(block[i].getGlobalBounds()) && !collision) {
                            collision = true;
                        }
                        if (Player_1.bro.getGlobalBounds().intersects(block[i].getGlobalBounds())) {
                            Player_1.bro.move(-velocity.x, 0);
                        }
                    }
                    if (!collision) {
                        Player_1.bro.move(-Player_1.speed * deltaTime, 0);
                        map.move(-Player_1.speed * deltaTime, 0);
                    }
                    else {
                        map.move(Player_1.speed * deltaTime, 0);
                    }
                }
                else {
                    Player_1.afk = true;
                    Player_1.bro.move(0, 0);
                    map.move(0, 0);
                }
                if (Player_1.status == 'D' && Player_1.afk) {
                    foot_steps.play();
                    if (Player_1.timer < 0) {
                        Player_1.bro.setTextureRect(IntRect(Player_1.boy_frame_afk * 19, 35, 19, 35));
                        Player_1.boy_frame_afk++;
                        Player_1.boy_frame_afk %= 8;
                        Player_1.timer = Player_1.delay;
                    }
                    else {
                        Player_1.timer -= deltaTime;
                    }
                    velocity.x = Player_1.speed * deltaTime;
                }
                else if (Player_1.status == 'A' && Player_1.afk) {
                    foot_steps.play();
                    if (Player_1.timer < 0) {
                        Player_1.bro.setTextureRect(IntRect(Player_1.boy_frame_afk * 19, 105, 19, 35));
                        Player_1.boy_frame_afk++;
                        Player_1.boy_frame_afk %= 8;
                        Player_1.timer = Player_1.delay;
                    }
                    else {
                        Player_1.timer -= deltaTime;
                    }
                    velocity.x = -Player_1.speed * deltaTime;
                }
                else if (Player_1.status == 'W' && Player_1.afk) {
                    foot_steps.play();
                    if (Player_1.timer < 0) {
                        Player_1.bro.setTextureRect(IntRect(Player_1.boy_frame_afk * 25, 70, 25, 35));
                        Player_1.boy_frame_afk++;
                        Player_1.boy_frame_afk %= 8;
                        Player_1.timer = Player_1.delay;
                    }
                    else {
                        Player_1.timer -= deltaTime;
                    }
                    velocity.y = -Player_1.speed * deltaTime;
                }
                else if (Player_1.status == 'S' && Player_1.afk) {
                    foot_steps.play();
                    if (Player_1.timer < 0) {
                        Player_1.bro.setTextureRect(IntRect(Player_1.boy_frame_afk * 26, 0, 26, 35));
                        Player_1.boy_frame_afk++;
                        Player_1.boy_frame_afk %= 8;
                        Player_1.timer = Player_1.delay;
                    }
                    else {
                        Player_1.timer -= deltaTime;
                    }
                    velocity.y = Player_1.speed * deltaTime;
                }
                else {
                    boy_breathing_out.play();
                }
            }
            //movement for map
            Vector2f viewCenter = map.getCenter();
            Vector2f playerPosition = Player_1.bro.getPosition();
            Vector2f viewSize = map.getSize();
            if (playerPosition.x - viewSize.x / 2 < 0) {
                viewCenter.x = viewSize.x / 2;
            }
            else if (playerPosition.x + viewSize.x / 2 > sprite_baackground.getGlobalBounds().width) {
                viewCenter.x = sprite_baackground.getGlobalBounds().width - viewSize.x / 2;
            }
            if (playerPosition.y - viewSize.y / 2 < 0) {
                viewCenter.y = viewSize.y / 2;
            }
            else if (playerPosition.y + viewSize.y / 2 > sprite_baackground.getGlobalBounds().height) {
                viewCenter.y = sprite_baackground.getGlobalBounds().height - viewSize.y / 2;
            }
            map.setCenter(viewCenter);

            //movement_coin 
            for (int i = 0; i < num_of_coins; i++) {
                if (timer_coin < 0) {
                    sprite_coin[i].setTextureRect(IntRect(currentframe_coin * 210, 0, 210, 209));
                    if (i == num_of_coins - 1) {
                        currentframe_coin++;
                        currentframe_coin %= 16;
                        timer_coin = delay_coin;
                    }
                }
                else {
                    i = num_of_coins - 1;
                    timer_coin -= deltaTime;
                }
            }

            FloatRect broBounds = Player_1.bro.getGlobalBounds();
            for (int i = 0; i < blocksnumbers - 2; i++) {
                if (broBounds.intersects(block[i].getGlobalBounds())) {
                    Vector2f correctionVector;

                    float xOverlap = min(broBounds.left + broBounds.width - block[i].getGlobalBounds().left,
                        block[i].getGlobalBounds().left + block[i].getGlobalBounds().width - broBounds.left);
                    float yOverlap = min(broBounds.top + broBounds.height - block[i].getGlobalBounds().top,
                        block[i].getGlobalBounds().top + block[i].getGlobalBounds().height - broBounds.top);

                    if (xOverlap < yOverlap) {
                        correctionVector.x = (broBounds.left < block[i].getGlobalBounds().left) ? -xOverlap : xOverlap;
                    }
                    else {
                        correctionVector.y = (broBounds.top < block[i].getGlobalBounds().top) ? -yOverlap : yOverlap;
                    }
                    Player_1.bro.move(correctionVector);
                }
            }

            if (COUNTER > 4) {
                for (int i = 49; i < blocksnumbers; i++) {
                    if (broBounds.intersects(block[i].getGlobalBounds())) {
                        Vector2f correctionVector;

                        float xOverlap = min(broBounds.left + broBounds.width - block[i].getGlobalBounds().left,
                            block[i].getGlobalBounds().left + block[i].getGlobalBounds().width - broBounds.left);
                        float yOverlap = min(broBounds.top + broBounds.height - block[i].getGlobalBounds().top,
                            block[i].getGlobalBounds().top + block[i].getGlobalBounds().height - broBounds.top);

                        if (xOverlap < yOverlap) {
                            correctionVector.x = (broBounds.left < block[i].getGlobalBounds().left) ? -xOverlap : xOverlap;
                        }
                        else {
                            correctionVector.y = (broBounds.top < block[i].getGlobalBounds().top) ? -yOverlap : yOverlap;
                        }
                        Player_1.bro.move(correctionVector);
                    }
                }
            }

            //collecting coins
            for (int counter_of_loops = 0; counter_of_loops < num_of_coins; ++counter_of_loops) {
                if (Player_1.bro.getGlobalBounds().intersects(sprite_coin[counter_of_loops].getGlobalBounds())) {
                    countr_slots_check++;
                    arr_draw[counter_of_loops] = false;
                    sprite_coin[counter_of_loops].setScale(0, 0);
                    if (countr_slots_check == 1)
                    {
                        texture_slots.loadFromFile("./assets/textures/slot 1.png");
                    }
                    else if (countr_slots_check == 2)
                    {
                        texture_slots.loadFromFile("./assets/textures/slot 2.png");
                    }
                    else if (countr_slots_check == 3)
                    {
                        texture_slots.loadFromFile("./assets/textures/slot 3.png");
                    }
                    else if (countr_slots_check == 4)
                    {
                        texture_slots.loadFromFile("./assets/textures/slot 4.png");
                    }
                    sprite_slots.setTexture(texture_slots);
                    coin_is_collected.play();
                }
            }

            // Timer decreasing as digits
            for (int counter = 0; counter < 59; counter++) {
                if (timer1.Timerclock.getElapsedTime().asSeconds() >= 1.55 && timer1.timer_of_pause > 0) {
                    timer1.timer_of_pause--;
                    if (timer1.timer_of_pause <= 20) {
                        timer1.texture_sand.loadFromFile("./assets/textures/timer_end.png");
                        timer1.sprite_sand.setTexture(timer1.texture_sand);
                        if (end_of_game.getStatus() == Music::Stopped)
                            end_of_game.play();
                    }
                    timer1.Timerclock.restart();
                }
                timer1.timerText.setString(to_string(timer1.timer_of_pause));
            }

            //timer condition sand clock
            if (timer1.currentframe_sand < 5 && timer1.counter_time <= 65) {
                if (timer1.timer_sand < 0) {
                    timer1.currentframe_sand++;
                    timer1.currentframe_sand = timer1.currentframe_sand % 5;
                    timer1.sprite_sand.setTextureRect(IntRect(timer1.currentframe_sand * 490, 0, 490, 348));
                    timer1.timer_sand = timer1.delay_sand;
                    timer1.counter_time++;
                }
                else
                    timer1.timer_sand -= deltaTime;
            }
            else {
                timer1.sprite_sand.setTextureRect(IntRect(0, 0, 490, 348));
            }

            //pyramid 
            if (timer7 < 0) {
                pyramid.setTextureRect(IntRect(p * 500, 0, 500, 475));
                p++;
                p %= 11;
                timer7 = 0.03;
            }
            else {
                timer7 -= deltaTime;
            }
            SShader.setPosition(Player_1.bro.getPosition().x + (Player_1.bro.getTextureRect().width / 2), Player_1.bro.getPosition().y + (Player_1.bro.getTextureRect().height / 2));
            mini_maze.setTextureRect(IntRect((map.getCenter().x - 960) * 0.09305555556, ((map.getCenter().y - 540) * 0.07120987654), 678 * 0.266, 543 * 0.266));

            //syntax
            if (TIME_R < 0) {
                COUNTER += 0.1;
                TIME_R = 0.1;
            }
            else {
                TIME_R -= deltaTime;
            }
            shape99.setSize(Vector2f(syntax.getGlobalBounds().width + 10, syntax.getGlobalBounds().height + 10));
            shape99.setOrigin((syntax.getGlobalBounds().width / 2.1) + 5, syntax.getGlobalBounds().height / 1.68);
            shape99.setPosition(syntax.getPosition());

            if (!hard && !replay_lvl1) {
                if (COUNTER > 3 && COUNTER <= 5.1)
                    text = "Escape the maze before the time runs out..";
                else if (COUNTER > 5.1 && COUNTER <= 6.6)
                    text = "or you'll die";
                else if (COUNTER > 6.6 && COUNTER <= 8.3)
                    text = "(Stefan) Don't worry i'll escape";
                else {
                    text = "";
                    shape99.setSize(Vector2f(0, 0));
                }
            }
            else if (!replay_lvl1) {
                if (COUNTER > 2.5 && COUNTER <= 5)
                    text = "Collect all the coins and escape the maze before the time runs out..";
                else if (COUNTER > 5 && COUNTER <= 6)
                    text = "or you'll die";
                else if (COUNTER > 6 && COUNTER <= 7)
                    text = "(Stefan) Don't worry i'll escape";
                else {
                    text = "";
                    shape99.setSize(Vector2f(0, 0));
                }
            }
            else {
                text = "";
                shape99.setSize(Vector2f(0, 0));
            }
            syntax.setString(text);
            syntax.setPosition(1920 / 2, 1025);
            syntax.setOrigin(syntax.getGlobalBounds().width / 2.1, syntax.getGlobalBounds().height / 1.68);

            if (COUNTER > 0) {
                window.clear();
                window.setView(map);
                window.draw(sprite_baackground);
                for (int counter = 0; counter < num_of_coins; ++counter) {
                    if (arr_draw[counter]) {
                        window.draw(sprite_coin[counter]);
                    }
                }
                window.draw(Player_1.bro);
                window.draw(sprite_Door);
                //for (int i = 0; i < blocksnumbers; i++) window.draw(block[i]);
                window.draw(SShader);
                window.setView(window.getDefaultView());
                window.draw(health[0]);
                window.draw(health[1]);
                window.draw(pyramid);
                for (int i = 0; i < 3; i++) window.draw(heart[i]);
                window.draw(health[2]);
                window.draw(sprite_slots);
                window.draw(timer1.sprite_sand);
                window.draw(timer1.timerText);
                window.draw(mini_maze);
                window.draw(minimap2);
                if (!replay_lvl1) {
                    window.draw(shape99);
                    window.draw(syntax);
                }
                window.display();
            }
            if (WIN) {
                YOU_WIN.play();
                gate_sound.play();
                coin_is_collected.stop();
                boy_breathing_out.stop();
                foot_steps.stop();
                boy_breathing_out.stop();
                end_of_game.stop();
                indoor_footsteps.stop();
                music_bg.stop();
                easy_mode_lvl1.stop();
                hard_mode_lvl1.stop();
                sleep(milliseconds(20));
                PageNumber = -7;
                winWindow(window, 2);
                if (PageNumber != 2) return;
            }
            deltaTime = c.getElapsedTime().asSeconds();
        }
    }
}

void gameWindow2(RenderWindow& window) {
    edit_data();
    const int shooting_max_delay = 13;
    RectangleShape black_screen(Vector2f(1920, 1080));
    black_screen.setFillColor(Color(0, 0, 0, 190));

    //ground
    RectangleShape rect(Vector2f(2000, 100));
    rect.setPosition(0, 910);
    rect.setFillColor(Color::Transparent);

    //background
    Texture b_g;
    b_g.loadFromFile("./assets/textures/map lvl2.png");
    Sprite background;
    background.setTexture(b_g);
    Texture b_g_out;
    b_g_out.loadFromFile("./assets/textures/map lvl2 ground.png");
    Texture b_g_outt;
    b_g_outt.loadFromFile("./assets/textures/map lvl2 ground2.png");
    Texture b_g_outtt;
    b_g_outtt.loadFromFile("./assets/textures/map lvl2 ground3.png");
    Sprite background_out;
    background_out.setTexture(b_g_out);
    Sprite background_outt;
    background_outt.setTexture(b_g_outt);
    Sprite background_outtt;
    background_outtt.setTexture(b_g_outtt);
    background_outt.setTextureRect(IntRect(0, 896, 16280, 1080));
    background_outt.setPosition(0, 896);
    Vector2f pos_bg_right_D;
    Vector2f pos_bg_left_A;

    //player
    Texture player_t;
    player_t.loadFromFile("./assets/textures/Hero_P1.png");
    Texture player_d;
    player_d.loadFromFile("./assets/textures/Hero_P2.png");
    Texture player_dd;
    player_dd.loadFromFile("./assets/textures/Sword_sprite2.png");
    Sprite bro;
    bro.setTexture(player_t);
    bro.setPosition(150, -150);
    Vector2f playerCenter;
    Vector2f pos_right_D;
    Vector2f pos_left_A;

    //player health
    Texture health_T[3];
    health_T[0].loadFromFile("./assets/textures/health.png");
    health_T[1].loadFromFile("./assets/textures/health2.png");
    health_T[2].loadFromFile("./assets/textures/health3.png");
    Sprite health[3];
    health[1].setPosition(141, 116);
    for (int i = 0; i < 3; i++) {
        health[i].setTexture(health_T[i]);
        health[i].setPosition(health[i].getPosition().x + 38, health[i].getPosition().y + 10);
        health[i].setScale(0.266, 0.266);
    }
    Texture heart_T;
    heart_T.loadFromFile("./assets/textures/heart.png");
    Sprite heart[3];
    for (int i = 0; i < 3; i++) {
        heart[i].setTexture(heart_T);
        heart[i].setTextureRect(IntRect(0, 0, 125.1, 126));
        heart[i].setScale(0.3, 0.3);
    }
    heart[2].setPosition(48.55, 125.5);
    heart[1].setPosition(48.55 + (126 * 0.3), 125.5);
    heart[0].setPosition(48.55 + (252 * 0.3), 125.5);

    //slots
    Texture slot_T[5];
    slot_T[0].loadFromFile("./assets/textures/slot 0.png");
    slot_T[1].loadFromFile("./assets/textures/slot 01.png");
    slot_T[2].loadFromFile("./assets/textures/slot 02.png");
    slot_T[3].loadFromFile("./assets/textures/slot 03.png");
    slot_T[4].loadFromFile("./assets/textures/slot 04.png");
    Sprite slot;
    slot.setTexture(slot_T[0]);
    slot.setPosition(slot.getPosition().x + 38, slot.getPosition().y + 10);
    slot.setScale(0.266, 0.266);

    //timer
    Texture timer_T[2];
    timer_T[0].loadFromFile("./assets/textures/timer.png");
    timer_T[1].loadFromFile("./assets/textures/timer_end.png");
    Sprite Time;
    Time.setTexture(timer_T[0]);
    Time.setScale(0.266, 0.266);
    Time.setTextureRect(IntRect(0, 0, 490, 348));
    Time.setPosition((1920 / 2) - (Time.getGlobalBounds().width / 2), Time.getPosition().y + 10);

    //timer Text
    Font font;
    font.loadFromFile("./assets/font/trebuc.ttf");
    Text TIME;
    TIME.setFont(font);
    TIME.setCharacterSize(50);
    TIME.setFillColor(Color::Black);
    TIME.setPosition((1920 / 2) - (Time.getGlobalBounds().width / 2) + 16, Time.getPosition().y + 7);

    //coins
    Texture coins_T;
    coins_T.loadFromFile("./assets/textures/coin 2 - NEW.png");
    Sprite coins[4];
    for (int i = 0; i < 4; i++) {
        coins[i].setTexture(coins_T);
        coins[i].setScale(0.2, 0.2);
        coins[i].setPosition(17000, 17000);
    }

    //mini map
    Texture mini;
    mini.loadFromFile("./assets/textures/mini map.png");
    Sprite minimap;
    minimap.setTexture(mini);
    minimap.setPosition(minimap.getPosition().x + 1680, minimap.getPosition().y + 10);
    minimap.setScale(0.266, 0.266);

    //mini map text
    Text map;
    map.setFont(font);
    map.setCharacterSize(40);;
    map.setOutlineThickness(1);
    map.setOutlineColor(Color::Black);
    Text map2;
    map2.setFont(font);
    map2.setCharacterSize(19);;
    map2.setFillColor(Color::Black);
    map2.setString("Next checkpoint in:");
    map2.setOrigin(map2.getGlobalBounds().width / 2, map2.getGlobalBounds().height / 2);
    map2.setPosition(1781, 62);

    //pyramid
    Texture pyramid_T;
    pyramid_T.loadFromFile("./assets/textures/pyramid.png");
    Sprite pyramid;
    pyramid.setTexture(pyramid_T);
    pyramid.setPosition(50, 20);
    pyramid.setScale(0.22, 0.3);

    //traps
    Texture trap_t[10];
    trap_t[0].loadFromFile("./assets/textures/trap0.png");
    trap_t[1].loadFromFile("./assets/textures/trap1.png");
    trap_t[2].loadFromFile("./assets/textures/trap2.png");
    trap_t[3].loadFromFile("./assets/textures/trap2.png");
    trap_t[4].loadFromFile("./assets/textures/trap2.png");
    trap_t[5].loadFromFile("./assets/textures/trap5.png");
    trap_t[6].loadFromFile("./assets/textures/trap5.png");
    trap_t[7].loadFromFile("./assets/textures/trap4.png");
    trap_t[8].loadFromFile("./assets/textures/trap6.png");
    trap_t[9].loadFromFile("./assets/textures/trap4.png");
    Sprite trap[10];
    for (int i = 0; i < 10; i++) {
        trap[i].setTexture(trap_t[i]);
    }
    trap[5].setPosition(0, 576);
    trap[6].setPosition(0, 965);
    trap[8].setScale(2, 2);
    trap[8].setTextureRect(IntRect(0, 0, 35, 33));
    //traps collision
    RectangleShape RECT[10];

    //shoothing
    Texture ammoT;
    ammoT.loadFromFile("./assets/textures/shoot.png");
    Sprite ammo[2];
    for (int i = 0; i < 2; i++) {
        ammo[i].setTexture(ammoT);
        ammo[i].setTextureRect(IntRect(0, 0, 250, 174));
    }
    ammo[0].setScale(0.1, 0.1);
    ammo[1].setScale(-0.1, 0.1);
    vector<Sprite> ammos_right;
    ammos_right.push_back(Sprite(ammo[0]));
    vector<Sprite> ammos_left;
    ammos_left.push_back(Sprite(ammo[1]));

    //cannon
    Texture cannonT;
    cannonT.loadFromFile("./assets/textures/cannon1.png");
    Sprite cannon;
    cannon.setTexture(cannonT);
    cannon.setTextureRect(IntRect(0, 0, 113, 77));
    cannon.setPosition(9150, 830);
    Texture ballT;
    ballT.loadFromFile("./assets/textures/cannon_ammo.png");
    Sprite ball;
    ball.setTexture(ballT);
    vector<Sprite> balls;
    balls.push_back(Sprite(ball));
    Vector2f cannonCenter;
    Texture cannonHealth_T[3];
    cannonHealth_T[0].loadFromFile("./assets/textures/cannon_health.png");
    cannonHealth_T[1].loadFromFile("./assets/textures/cannon_health3.png");
    cannonHealth_T[2].loadFromFile("./assets/textures/cannon_health2.png");
    Sprite cannonHealth[3];
    for (int i = 0; i < 3; i++) {
        cannonHealth[i].setTexture(cannonHealth_T[i]);
    }

    //enemy
    Texture enemy_t;
    Texture enemy_td;
    enemy_t.loadFromFile("./assets/textures/skull.png");
    enemy_td.loadFromFile("./assets/textures/skull_0.png");
    Sprite enemy;
    enemy.setTexture(enemy_t);
    enemy.setTextureRect(IntRect(0, 0, 104, 128));
    enemy.setScale(0.8, 0.8);
    enemy.setPosition(2023, 798);
    enemy.setOrigin(enemy.getTextureRect().width / 2, enemy.getTextureRect().height / 2);
    Vector2f enemyCenter;
    Texture enemyHealth_T[3];
    enemyHealth_T[0].loadFromFile("./assets/textures/skull_health.png");
    enemyHealth_T[1].loadFromFile("./assets/textures/skull_health3.png");
    enemyHealth_T[2].loadFromFile("./assets/textures/skull_health2.png");
    Sprite enemyHealth[3];
    for (int i = 0; i < 3; i++) {
        enemyHealth[i].setTexture(enemyHealth_T[i]);
        enemyHealth[i].setScale(0.8, 0.8);
        enemyHealth[i].setOrigin(enemyHealth[i].getTextureRect().width / 2 - 28, enemyHealth[i].getTextureRect().height / 2);
    }
    enemyHealth[1].setOrigin(0, 0);

    //neon 
    Texture neonT;
    neonT.loadFromFile("./assets/textures/Neon.png");
    Sprite neon[2];
    for (int i = 0; i < 2; i++) {
        neon[i].setTexture(neonT);
        neon[i].setScale(0.2, 0.2);
    }

    //gate
    Texture gate_T;
    gate_T.loadFromFile("./assets/textures/gate.png");
    Sprite gate;
    gate.setTexture(gate_T);
    gate.setScale(1.1, 1.1);
    Texture gate2_T;
    gate2_T.loadFromFile("./assets/textures/gate 2.png");
    Sprite gate2;
    gate2.setTexture(gate2_T);
    gate2.setScale(1.1, 1.1);

    //fire
    Texture fire_t;
    fire_t.loadFromFile("./assets/textures/torch.png");
    Sprite fire[22];
    for (int i = 0; i < 22; i++) {
        fire[i].setTexture(fire_t);
        fire[i].setScale(0.4, 0.4);
    }

    //syntax
    String text;
    Text syntax;
    syntax.setFont(font);
    syntax.setCharacterSize(25);
    RectangleShape shape;
    shape.setFillColor(Color(0, 0, 0, 140));
    Music talk;
    talk.openFromFile("./assets/sounds/lvl 2 #1.mp3");
    if (!replay_lvl2)  talk.play();

    //music
    Music music_bg;
    music_bg.openFromFile("./assets/sounds/bg_music_FULL.mp3");
    music_bg.setVolume(28 * volume);
    music_bg.setLoop(true);
    Music foot[2];
    foot[0].openFromFile("./assets/sounds/foot.mp3");
    foot[0].setVolume(44);
    foot[0].setLoop(true);
    foot[1].openFromFile("./assets/sounds/foot 2x.mp3");
    foot[1].setVolume(44);
    foot[1].setLoop(true);
    Music shoot;
    shoot.openFromFile("./assets/sounds/shoot.mp3");
    shoot.setVolume(60);
    shoot.setLoop(true);
    Music shoot2;
    shoot2.openFromFile("./assets/sounds/shoot1.mp3");
    shoot2.setVolume(70);
    shoot2.setLoop(true);
    Music die;
    die.openFromFile("./assets/sounds/die0.mp3");
    die.setVolume(55);
    Music die_cannon;
    die_cannon.openFromFile("./assets/sounds/die_cannon.mp3");
    die_cannon.setVolume(55);
    Music jump;
    jump.openFromFile("./assets/sounds/jump.mp3");
    jump.setVolume(69);
    Music knife;
    knife.openFromFile("./assets/sounds/knife.mp3");
    knife.setVolume(30);
    Music punch;
    punch.openFromFile("./assets/sounds/punch.mp3");
    punch.setVolume(10);
    Music hurt;
    hurt.openFromFile("./assets/sounds/hurt.mp3");
    hurt.setVolume(100);
    Music coin;
    coin.openFromFile("./assets/sounds/coin collected.wav");
    coin.setVolume(89);
    Music enemy_sound;
    enemy_sound.openFromFile("./assets/sounds/you cant kill.mp3");
    enemy_sound.setVolume(65);
    Music you_are_dead[2];
    you_are_dead[0].openFromFile("./assets/sounds/YOU ARE DEAD 2.wav");
    you_are_dead[1].openFromFile("./assets/sounds/you-re DEAD 2.wav");
    Music gate_sound;
    gate_sound.openFromFile("./assets/sounds/gate.mp3");
    Music game_over_sound;
    game_over_sound.openFromFile("./assets/sounds/game_over_sound.mp3");
    Music end_of_game;
    end_of_game.openFromFile("./assets/sounds/end_of_game (1).mp3");
    end_of_game.setVolume(39);

    Clock c;
    Vector2f pos_jump(0, 0);
    float deltaTime = 0, timer = 0, timer2 = 0, timer3 = 0, timer4 = 0, timer5 = 0, timer6 = 0, timer7 = 0, timer8 = 0, timer9 = 0, timer10 = 0, timer11 = 0, timer100 = 0, gravity = 62, jumphight = 22, timer666 = 0;
    int vii = 0, ii = 0, iii = 0, iv = 0, f = 0, k = 0, t = 0, Cc = 0, TT = 0, p = 0, speed = 250, Mspeed = 400, speedT = 50, Fpos = 500, TIME_Display = 100, coin_collect = 0, heart_anime = 0, heart_num = 0;
    int shoot_delay = 0, shoot_delay_2 = 0, shoot_speed = 550, shoot_animation = 0, COUNTER = 0, i_c = 0, i_gate = 0, enemies_anime = 0, enemies_anime_d = 0, enemy_speed = 150, coins_collected = 0;
    bool draw_health_enemy = true, left = true, enemy_damaged = 0;
    bool level_3 = false;
    bool grounded = true;
    bool checkT = false;
    bool dead = false;
    char status = 'D';
    int pause_count = 0;
    if (sound) music_bg.play();
    while (window.isOpen()) {
        if (PageNumber == 3 || PageNumber == 4) level_3 = true;
        else if (PageNumber != 2) break;
        bool play = true;
        c.restart();
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    while (pause_count < 2) {
                        pause_count++;
                        window.draw(black_screen);
                        window.display();
                    }
                    pause_count = 0;
                    music_bg.pause();
                    if (talk.getStatus() == Music::Playing) talk.pause();
                    if (foot[0].getStatus() == Music::Playing) foot[0].pause();
                    if (foot[1].getStatus() == Music::Playing) foot[1].pause();
                    if (shoot.getStatus() == Music::Playing) shoot.pause();
                    if (shoot2.getStatus() == Music::Playing) shoot2.pause();
                    if (die.getStatus() == Music::Playing) die.pause();
                    if (jump.getStatus() == Music::Playing) jump.pause();
                    if (knife.getStatus() == Music::Playing) knife.pause();
                    if (punch.getStatus() == Music::Playing) punch.pause();
                    if (hurt.getStatus() == Music::Playing) hurt.pause();
                    if (coin.getStatus() == Music::Playing) coin.pause();
                    if (enemy_sound.getStatus() == Music::Playing) enemy_sound.pause();
                    if (you_are_dead[0].getStatus() == Music::Playing) you_are_dead[0].pause();
                    if (you_are_dead[1].getStatus() == Music::Playing) you_are_dead[1].pause();
                    if (gate_sound.getStatus() == Music::Playing) gate_sound.pause();
                    if (game_over_sound.getStatus() == Music::Playing) game_over_sound.pause();
                    if (end_of_game.getStatus() == Music::Playing) end_of_game.pause();
                    pause(window);
                    if (PageNumber != 2) return;
                    if (sound) music_bg.play();
                    if (talk.getStatus() == Music::Paused) talk.play();
                    if (foot[0].getStatus() == Music::Paused) foot[0].play();
                    if (foot[1].getStatus() == Music::Paused) foot[1].play();
                    if (shoot.getStatus() == Music::Paused) shoot.play();
                    if (shoot2.getStatus() == Music::Paused) shoot2.play();
                    if (die.getStatus() == Music::Paused) die.play();
                    if (jump.getStatus() == Music::Paused) jump.play();
                    if (knife.getStatus() == Music::Paused) knife.play();
                    if (punch.getStatus() == Music::Paused) punch.play();
                    if (hurt.getStatus() == Music::Paused) hurt.play();
                    if (coin.getStatus() == Music::Paused) coin.play();
                    if (enemy_sound.getStatus() == Music::Paused) enemy_sound.play();
                    if (you_are_dead[0].getStatus() == Music::Paused) you_are_dead[0].play();
                    if (you_are_dead[1].getStatus() == Music::Paused) you_are_dead[1].play();
                    if (gate_sound.getStatus() == Music::Paused) gate_sound.play();
                    if (game_over_sound.getStatus() == Music::Paused) game_over_sound.play();
                    if (end_of_game.getStatus() == Music::Paused) end_of_game.play();
                    play = false;
                    break;
                }
            }
        }
        if (play) {
            if (bro.getPosition().x < 300 && background.getPosition().x < 300 && background.getPosition().x > 0) background.setPosition(0, 0);
            bool intersects = false;
            bool intersects_T = false;
            //player animation
            bool AFK = true;
            bro.setScale(3, 3);
            if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::LShift) && grounded) {
                shoot.stop();
                if (foot[1].getStatus() == Music::Stopped) foot[1].play();
                foot[0].stop();
                pos_left_A.x = 0;
                pos_bg_left_A.x = 0;
                if (timer < 0) {
                    bro.setTextureRect(IntRect(vii * 29, 283, 29, 30));
                    vii++;
                    vii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'D';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    pos_bg_right_D.x = -speed * 2 * deltaTime;
                    pos_right_D.x = 20 * deltaTime;
                }
                else {
                    pos_right_D.x = speed * 2 * deltaTime;
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::LShift) && grounded) {
                shoot.stop();
                if (foot[1].getStatus() == Music::Stopped) foot[1].play();
                foot[0].stop();
                pos_right_D.x = 0;
                pos_bg_right_D.x = 0;
                if (timer < 0) {
                    bro.setTextureRect(IntRect(vii * 29, 313, 29, 30));
                    vii++;
                    vii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'A';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    pos_bg_left_A.x = speed * 2 * deltaTime;
                    pos_left_A.x = -20 * deltaTime;
                }
                else {
                    pos_left_A.x = -speed * 2 * deltaTime;
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::D) && Mouse::isButtonPressed(Mouse::Left)) {
                if (shoot.getStatus() == Music::Stopped && COUNTER > 8) shoot.play();
                if (grounded) {
                    if (foot[0].getStatus() == Music::Stopped) foot[0].play();
                    foot[1].stop();
                }
                else {
                    foot[0].stop();
                    foot[1].stop();
                }
                pos_left_A.x = 0;
                pos_bg_left_A.x = 0;
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iv * 22, 343, 22, 32));
                    iv++;
                    iv %= 6;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'D';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    pos_bg_right_D.x = -speed * deltaTime;
                    pos_right_D.x = 20 * deltaTime;
                }
                else {
                    pos_right_D.x = speed * deltaTime;
                }
                //shooting D
                if (shoot_delay > shooting_max_delay) {
                    shoot_delay = 0;
                    ammo[0].setPosition(playerCenter);
                    ammos_right.push_back(Sprite(ammo[0]));
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::A) && Mouse::isButtonPressed(Mouse::Left)) {
                if (shoot.getStatus() == Music::Stopped && COUNTER > 8) shoot.play();
                if (grounded) {
                    if (foot[0].getStatus() == Music::Stopped) foot[0].play();
                    foot[1].stop();
                }
                else {
                    foot[0].stop();
                    foot[1].stop();
                }
                pos_right_D.x = 0;
                pos_bg_right_D.x = 0;
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iv * 22, 375, 22, 32));
                    iv++;
                    iv %= 6;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'A';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    pos_bg_left_A.x = speed * deltaTime;
                    pos_left_A.x = -20 * deltaTime;
                }
                else {
                    pos_left_A.x = -speed * deltaTime;
                }
                //shooting A
                if (shoot_delay > shooting_max_delay) {
                    shoot_delay = 0;
                    ammo[1].setPosition(playerCenter);
                    ammos_left.push_back(Sprite(ammo[1]));
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::D)) {
                shoot.stop();
                if (grounded) {
                    if (foot[0].getStatus() == Music::Stopped) foot[0].play();
                    foot[1].stop();
                }
                else {
                    foot[0].stop();
                    foot[1].stop();
                }
                pos_left_A.x = 0;
                pos_bg_left_A.x = 0;
                if (timer < 0) {
                    bro.setTextureRect(IntRect(ii * 20, 175, 20, 35));
                    ii++;
                    ii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'D';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    pos_bg_right_D.x = -speed * deltaTime;
                    pos_right_D.x = 20 * deltaTime;
                }
                else {
                    pos_right_D.x = speed * deltaTime;
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::A)) {
                shoot.stop();
                if (grounded) {
                    if (foot[0].getStatus() == Music::Stopped) foot[0].play();
                    foot[1].stop();
                }
                else {
                    foot[0].stop();
                    foot[1].stop();
                }
                pos_right_D.x = 0;
                pos_bg_right_D.x = 0;
                if (timer < 0) {
                    bro.setTextureRect(IntRect(ii * 20, 248, 20, 35));
                    ii++;
                    ii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'A';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    pos_bg_left_A.x = speed * deltaTime;
                    pos_left_A.x = -20 * deltaTime;
                }
                else {
                    pos_left_A.x = -speed * deltaTime;
                }
            }
            else if (status == 'D' && Mouse::isButtonPressed(Mouse::Left) && COUNTER > 8) {
                if (shoot.getStatus() == Music::Stopped) shoot.play();
                foot[0].stop();
                foot[1].stop();
                pos_right_D.x = 0;
                pos_bg_right_D.x = 0;
                pos_left_A.x = 0;
                pos_bg_left_A.x = 0;
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iv * 22, 407, 22, 33));
                    iv++;
                    iv %= 6;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'D';
                AFK = false;
                //shooting D (not moving)
                if (shoot_delay > shooting_max_delay) {
                    shoot_delay = 0;
                    ammo[0].setPosition(playerCenter);
                    ammos_right.push_back(Sprite(ammo[0]));
                }
            }
            else if (status == 'A' && Mouse::isButtonPressed(Mouse::Left) && COUNTER > 8) {
                if (shoot.getStatus() == Music::Stopped) shoot.play();
                foot[0].stop();
                foot[1].stop();
                pos_right_D.x = 0;
                pos_bg_right_D.x = 0;
                pos_left_A.x = 0;
                pos_bg_left_A.x = 0;
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iv * 22, 440, 22, 33));
                    iv++;
                    iv %= 6;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'A';
                AFK = false;
                //shooting A (not moving)
                if (shoot_delay > shooting_max_delay) {
                    shoot_delay = 0;
                    ammo[1].setPosition(playerCenter);
                    ammos_left.push_back(Sprite(ammo[1]));
                }
            }
            else {
                shoot.stop();
                foot[0].stop();
                foot[1].stop();
                pos_bg_right_D.x = 0;
                pos_bg_left_A.x = 0;
                pos_right_D.x = 0;
                pos_left_A.x = 0;
            }

            //trap collision
            FloatRect broBounds = bro.getGlobalBounds();
            for (int i = 0; i < 10; i++) {
                if (broBounds.intersects(RECT[i].getGlobalBounds())) {
                    Vector2f correctionVector;
                    float xOverlap = min(broBounds.left + broBounds.width - RECT[i].getGlobalBounds().left,
                        RECT[i].getGlobalBounds().left + RECT[i].getGlobalBounds().width - broBounds.left);
                    float yOverlap = min(broBounds.top + broBounds.height - RECT[i].getGlobalBounds().top,
                        RECT[i].getGlobalBounds().top + RECT[i].getGlobalBounds().height - broBounds.top);
                    if (xOverlap < yOverlap) {
                        correctionVector.x = (broBounds.left < RECT[i].getGlobalBounds().left) ? -xOverlap : xOverlap;
                        pos_bg_left_A.x = 0;
                        pos_bg_right_D.x = 0;
                        if (i < 5) intersects = true;
                    }
                    else {
                        correctionVector.y = (broBounds.top < RECT[i].getGlobalBounds().top) ? -yOverlap : yOverlap;
                        intersects_T = true;
                        intersects = true;
                    }
                    if (broBounds.intersects(RECT[8].getGlobalBounds())) {
                        intersects_T = false;
                        intersects = true;
                    }
                    bro.move(correctionVector);
                }
            }

            //pos movement 
            if (background.getPosition().x > -11430.6) {
                for (int i = 0; i < ammos_right.size(); i++) {
                    ammos_right[i].move(pos_bg_left_A.x, 0);
                }
                for (int i = 0; i < ammos_left.size(); i++) {
                    ammos_left[i].move(pos_bg_right_D.x, 0);
                }
                for (int i = 0; i < balls.size(); i++) {
                    balls[i].move(pos_bg_right_D.x, 0);
                }
                if (COUNTER > 12) {
                    enemy.move(pos_bg_right_D.x, 0);
                    enemy.move(pos_bg_left_A.x, 0);
                }
                background.move(pos_bg_right_D);
                background.move(pos_bg_left_A);
            }
            bro.move(pos_right_D);
            bro.move(pos_left_A);

            //player AFK movement
            if (status == 'D' && AFK) {
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iii * 19, 35, 19, 35));
                    iii++;
                    iii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
            }
            else if (status == 'A' && AFK) {
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iii * 19, 105, 19, 35));
                    iii++;
                    iii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
            }

            //cannon
            shoot_delay_2++;
            cannonCenter = Vector2f((cannon.getPosition().x + (cannon.getGlobalBounds().width / 2)) - 70, (cannon.getPosition().y + (cannon.getGlobalBounds().height / 2)) - 22);
            for (int i = 0; i < balls.size(); i++) {
                balls[i].move(-shoot_speed * deltaTime, 0);
                if (abs(balls[i].getPosition().x - cannon.getPosition().x) > 700 || balls[i].getPosition().x < 0)
                    balls.erase(balls.begin() + i);
            }
            if (cannonHealth[1].getScale().x <= 0.0079) {
                trap[9].move(0, -speed * 0.6 * deltaTime);
                ball.setScale(0, 0);
                cannon.setTextureRect(IntRect(4 * 113, 0, 113, 77));
                if ((die_cannon.getStatus() == Music::Stopped)) {
                    die_cannon.play();
                }
                shoot2.stop();
            }
            else {
                if (timer10 < 0) {
                    i_c++;
                    i_c %= 4;
                    cannon.setTextureRect(IntRect(i_c * 113, 0, 113, 77));
                    timer10 = 0.09;
                }
                else {
                    timer10 -= deltaTime;
                }
                if (shoot_delay_2 > shooting_max_delay) {
                    shoot_delay_2 = 0;
                    ball.setPosition(cannonCenter);
                    balls.push_back(Sprite(ball));
                }
                if (trap[5].getPosition().y > 909) {
                    cannon.setTextureRect(IntRect(0, 0, 113, 77));
                    i_c = 4;
                    ball.setScale(0, 0);
                    shoot2.stop();
                }
                else {
                    if ((shoot2.getStatus() == Music::Stopped)) {
                        if (cannon.getPosition().x - bro.getPosition().x < 1600) {
                            shoot2.play();
                        }
                        else {
                            shoot2.stop();
                        }
                    }
                    ball.setScale(1, 1);
                }
                // cannon health bar
                for (int i = 0; i < ammos_right.size(); i++) {
                    if (abs((cannonCenter.x + 35) - ammos_right[i].getPosition().x) < 7 && COUNTER > 8) {
                        if (hard) {
                            cannonHealth[1].scale(0.8, 1);
                            if (cannonHealth[1].getScale().x <= 0.44)
                                cannonHealth[1].scale(0.78, 1);
                        }
                        else {
                            cannonHealth[1].scale(0.6, 1);
                            if (cannonHealth[1].getScale().x <= 0.44)
                                cannonHealth[1].scale(0.5, 1);
                        }
                        ammos_right.erase(ammos_right.begin() + i);
                    }
                }
                for (int i = 0; i < ammos_left.size(); i++) {
                    if (abs((cannonCenter.x + 35) - ammos_left[i].getPosition().x) < 7 && COUNTER > 8) {
                        if (hard) {
                            cannonHealth[1].scale(0.8, 1);
                            if (cannonHealth[1].getScale().x <= 0.44)
                                cannonHealth[1].scale(0.78, 1);
                        }
                        else {
                            cannonHealth[1].scale(0.6, 1);
                            if (cannonHealth[1].getScale().x <= 0.44)
                                cannonHealth[1].scale(0.5, 1);
                        }
                        ammos_left.erase(ammos_left.begin() + i);
                    }
                }
            }

            //shooting
            shoot_delay++;
            playerCenter = Vector2f(bro.getPosition().x + (bro.getGlobalBounds().width / 2), bro.getPosition().y + (bro.getGlobalBounds().height / 2));
            for (int i = 0; i < ammos_right.size(); i++) {
                if (timer9 < 0) {
                    ammos_right[i].setTextureRect(IntRect(shoot_animation * 250, 0, 250, 174));
                    if (i == ammos_right.size() - 1) {
                        shoot_animation++;
                        shoot_animation %= 4;
                        timer9 = 0.15;
                    }
                }
                else {
                    i = ammos_right.size() - 1;
                    timer9 -= deltaTime;
                }
            }
            for (int i = 0; i < ammos_left.size(); i++) {
                if (timer9 < 0) {
                    ammos_left[i].setTextureRect(IntRect(shoot_animation * 250, 0, 250, 174));
                    if (i == ammos_left.size() - 1) {
                        shoot_animation++;
                        shoot_animation %= 4;
                        timer9 = 0.15;
                    }
                }
                else {
                    i = ammos_left.size() - 1;
                    timer9 -= deltaTime;
                }
            }
            for (int i = 0; i < ammos_right.size(); i++) {
                ammos_right[i].move(shoot_speed * deltaTime, 0);
                if (ammos_right[i].getPosition().x > (bro.getPosition().x + 600) || ammos_right[i].getPosition().x < 0)
                    ammos_right.erase(ammos_right.begin() + i);
            }
            for (int i = 0; i < ammos_left.size(); i++) {
                ammos_left[i].move(-shoot_speed * deltaTime, 0);
                if (ammos_left[i].getPosition().x > (bro.getPosition().x + 600) || ammos_left[i].getPosition().x < 0)
                    ammos_left.erase(ammos_left.begin() + i);
            }

            //fire movement
            for (int i = 0; i < 22; i++) {
                if (timer3 < 0) {
                    fire[i].setTextureRect(IntRect(f * 167, 0, 167, 342));
                    if (i == 21) {
                        f++;
                        f %= 7;
                        timer3 = 0.15;
                    }
                }
                else {
                    i = 21;
                    timer3 -= deltaTime;
                }
            }

            //player jump & gravity
            if ((broBounds.intersects(rect.getGlobalBounds()))) {
                speed = 250;
                pos_jump.y = 0;
                grounded = true;
            }
            else {
                speed = 400;
                pos_jump.y += gravity * deltaTime;
            }

            if (Keyboard::isKeyPressed(Keyboard::Space) && grounded)
            {
                jump.play();
                pos_jump.y -= jumphight;
                grounded = false;
            }
            bro.move(pos_jump);


            //traps movement
            if (timer4 < 0) {
                trap[5].setTextureRect(IntRect(0, 0, 269, 327 - (trap[5].getPosition().y - 576)));
                trap[6].setTextureRect(IntRect(0, 0, 269, 327 - (trap[6].getPosition().y - 965)));
                k++;
                k %= 82;
                timer4 = 0.1;
                if (k == 81) {
                    speedT *= -1;
                }
            }
            else {
                timer4 -= deltaTime;
            }
            if (trap[5].getPosition().y > 909) {
                neon[0].setTextureRect(IntRect(0, 0, 1314, 720));
                neon[1].setTextureRect(IntRect(1314, 0, 1314, 720));
            }
            else {
                neon[0].setTextureRect(IntRect(1314, 0, 1314, 720));
                neon[1].setTextureRect(IntRect(0, 0, 1314, 720));
            }
            if (trap[6].getPosition().y > 925) {
                neon[1].setTextureRect(IntRect(0, 0, 1314, 720));
            }
            else {
                neon[1].setTextureRect(IntRect(1314, 0, 1314, 720));
            }
            trap[5].move(0, speedT * deltaTime);
            trap[6].move(0, -speedT * deltaTime);
            trap[7].move(0, -speedT * 1.5 * deltaTime);
            if (bro.getGlobalBounds().intersects(trap[8].getGlobalBounds()))
                checkT = true;

            if (checkT) {
                if (timer8 < 0) {
                    if (TT <= 4)
                        trap[8].setTextureRect(IntRect(TT * 35, 0, 35, 33));
                    timer8 = 0.1;
                    TT++;
                }
                else if (TT == 7) {
                    trap[8].setScale(0, 0);
                    RECT[8].setScale(0, 0);
                    trap[8].setTextureRect(IntRect(0, 0, 0, 0));
                    checkT = false;
                }
                else {
                    timer8 -= deltaTime;
                }
            }


            //enemies
            enemyCenter = Vector2f((enemy.getPosition().x + (enemy.getGlobalBounds().width / 2)) - 70, (enemy.getPosition().y + (enemy.getGlobalBounds().height / 2)) - 22);
            enemy_damaged = false;
            if (enemy.getPosition().x < -110) {
                enemies_anime_d = 0;
                if (hard)
                    enemy.setPosition(2023, 850);
                else
                    enemy.setPosition(2350, 850);
                enemies_anime_d = 0;
                enemyHealth[1].setScale(0.8, 0.8);
            }
            if (enemyHealth[1].getScale().x <= 0.04) {
                knife.stop();
                if (die.getStatus() == Music::Stopped) die.play();
                enemy.setPosition(enemy.getPosition().x, 812);
                enemy.setTexture(enemy_t);
                draw_health_enemy = false;
                if (timer100 < 0) {
                    enemy.setTextureRect(IntRect(enemies_anime_d * 204, 436, 204, 192));
                    enemies_anime_d %= 12;
                    enemies_anime_d++;
                    timer100 = 0.15;
                }
                else {
                    timer100 -= deltaTime;
                }
                if (enemies_anime_d == 11) {
                    enemy.setTexture(enemy_t);
                    enemies_anime_d = 0;
                    if (hard)
                        enemy.setPosition(2023, 850);
                    else
                        enemy.setPosition(2350, 850);
                    enemyHealth[1].setScale(0.8, 0.8);
                }
            }
            else if (!enemy.getGlobalBounds().intersects(bro.getGlobalBounds())) {
                knife.stop();
                die.stop();
                if (left) {
                    enemyHealth[0].setPosition(enemy.getPosition().x + 16, 798 - 40);
                    enemyHealth[1].setPosition(enemy.getPosition().x + 16 + 16.8 - 38, 798 + 8 - 40 - 21);
                    enemyHealth[2].setPosition(enemy.getPosition().x + 16, 798 - 40);
                }
                else {
                    enemyHealth[0].setPosition(enemy.getPosition().x - 56, 798 - 40);
                    enemyHealth[1].setPosition(enemy.getPosition().x - 56 + 16.8 - 38, 798 + 8 - 40 - 21);
                    enemyHealth[2].setPosition(enemy.getPosition().x - 56, 798 - 40);
                }
                if (COUNTER > 12) {
                    enemy.move(enemy_speed * deltaTime, 0);
                }
                enemy.setPosition(enemy.getPosition().x, 850);
                if (timer100 < 0) {
                    enemy.setTextureRect(IntRect(enemies_anime * 144, 128, 144, 144));
                    enemies_anime++;
                    enemies_anime %= 16;
                    timer100 = 0.08;
                }
                else {
                    timer100 -= deltaTime;
                }
                draw_health_enemy = true;
            }
            else {
                if (knife.getStatus() == Music::Stopped && COUNTER > 8) knife.play();
                die.stop();
                intersects = true;
                if (left) {
                    enemyHealth[0].setPosition(enemy.getPosition().x + 42, 798 - 40);
                    enemyHealth[1].setPosition(enemy.getPosition().x + 42 + 16.8 - 38, 798 + 8 - 40 - 21);
                    enemyHealth[2].setPosition(enemy.getPosition().x + 42, 798 - 40);
                }
                enemy.setPosition(enemy.getPosition().x, 832);
                if (timer100 < 0) {
                    enemy.setTextureRect(IntRect(enemies_anime * 180, 272, 180, 164));
                    enemies_anime++;
                    enemies_anime %= 10;
                    timer100 = 0.08;
                }
                else {
                    timer100 -= deltaTime;
                }
                draw_health_enemy = true;
            }
            if (enemies_anime_d == 0)
            {
                for (int i = 0; i < ammos_right.size(); i++) {
                    if (abs((enemyCenter.x + 35) - ammos_right[i].getPosition().x) < 7 && COUNTER > 8) {
                        if (hard) {
                            enemyHealth[1].scale(0.7, 1);
                            if (enemyHealth[1].getScale().x <= 0.32)
                                enemyHealth[1].scale(0.69, 1);
                        }
                        else {
                            enemyHealth[1].scale(0.56, 1);
                            if (enemyHealth[1].getScale().x <= 0.32)
                                enemyHealth[1].scale(0.5, 1);
                        }
                        ammos_right[i].setColor(Color::Transparent);
                    }
                    if (ammos_right[i].getGlobalBounds().intersects(enemy.getGlobalBounds())) {
                        enemy_damaged = true;
                        break;
                    }
                }
                for (int i = 0; i < ammos_left.size(); i++) {
                    if (abs((enemyCenter.x + 35) - ammos_left[i].getPosition().x) < 7 && COUNTER > 8) {
                        if (hard) {
                            enemyHealth[1].scale(0.7, 1);
                            if (enemyHealth[1].getScale().x <= 0.32)
                                enemyHealth[1].scale(0.69, 1);
                        }
                        else {
                            enemyHealth[1].scale(0.56, 1);
                            if (enemyHealth[1].getScale().x <= 0.32)
                                enemyHealth[1].scale(0.5, 1);
                        }
                        ammos_left[i].setColor(Color::Transparent);
                    }
                    if (ammos_left[i].getGlobalBounds().intersects(enemy.getGlobalBounds())) {
                        enemy_damaged = true;
                        break;
                    }
                }
                if (bro.getPosition().x > enemy.getPosition().x) {
                    left = false;
                    enemy_speed = 160;
                    enemy.setScale(-0.8, 0.8);
                }
                else {
                    left = true;
                    enemy_speed = -160;
                    enemy.setScale(0.8, 0.8);
                }
                if (enemy_damaged) {
                    if (punch.getStatus() == Music::Stopped) punch.play();
                    enemy.setTexture(enemy_td);
                }
                else {
                    enemy.setTexture(enemy_t);
                }
            }
            if (COUNTER > 30 && enemy.getPosition().x > 2020) {
                if (enemy_sound.getStatus() == Music::Stopped)
                    if ((you_are_dead[0].getStatus() == Music::Stopped) && (you_are_dead[1].getStatus() == Music::Stopped))
                        enemy_sound.play();
            }

            //player damage
            for (int i = 0; i < balls.size(); ++i) {
                if (bro.getGlobalBounds().intersects(balls[i].getGlobalBounds())) {
                    intersects = true;
                    break;
                }
            }
            if (intersects) {
                if (!grounded) { jump.stop(); }
                if (hurt.getStatus() == Music::Stopped && COUNTER > 8) {
                    hurt.play();
                }
                bro.setTexture(player_d);
                if (timer5 < 0) {
                    timer5 = 0.4;
                    if (!intersects_T) {
                        if (hard) {
                            health[1].scale(0.6, 1);
                            if (health[1].getScale().x < 0.33)
                                health[1].scale(0.6, 1);
                        }
                        else {
                            health[1].scale(0.7, 1);
                            if (health[1].getScale().x < 0.33)
                                health[1].scale(0.6, 1);
                        }
                    }
                    else {
                        health[1].scale(0.15, 1);
                        if (health[1].getScale().x < 0.53)
                            health[1].scale(0.1, 1);
                    }
                }
                else {
                    timer5 -= deltaTime;
                }
            }
            else {
                hurt.stop();
                bro.setTexture(player_t);
            }
            //hearts
            if (health[1].getScale().x <= 0.008) {
                if (heart_num < 3) {
                    you_are_dead[rand() % 2].play();
                    talk.stop();
                    ii = 4;
                    bro.setTexture(player_dd);
                    bro.setPosition(bro.getPosition().x - 70, bro.getPosition().y - 58);
                    while (ii < 9) {
                        if (timer < 0) {
                            bro.setScale(2, 2);
                            bro.setTextureRect(IntRect(ii * 92, 0, 92, 102));
                            ii++;
                            timer = 0.07;
                        }
                        else {
                            timer -= deltaTime;
                        }
                        window.clear();
                        window.draw(background);
                        for (int i = 0; i < 22; i++) {
                            fire[i].setPosition(background.getPosition().x + Fpos, 760);
                            Fpos += 500;
                            window.draw(fire[i]);
                            if (i == 21) Fpos = 500;
                        }
                        window.draw(background_out);
                        for (int i = 2; i < 10; i++) window.draw(trap[i]);
                        window.draw(background_outt);
                        window.draw(neon[0]);
                        window.draw(neon[1]);
                        for (int i = 0; i < 4; i++) window.draw(coins[i]);
                        for (int i = 0; i < 3; i++) window.draw(cannonHealth[i]);
                        if (draw_health_enemy) for (int i = 0; i < 3; i++) window.draw(enemyHealth[i]);
                        window.draw(enemy);
                        window.draw(cannon);
                        window.draw(gate);
                        window.draw(bro);
                        window.draw(background_outtt);
                        window.draw(gate2);
                        for (int i = 0; i < 2; i++) window.draw(trap[i]);
                        window.draw(health[0]);
                        window.draw(health[1]);
                        window.draw(pyramid);
                        for (int i = 0; i < 3; i++) window.draw(heart[i]);
                        window.draw(health[2]);
                        window.draw(slot);
                        window.draw(Time);
                        window.draw(TIME);
                        window.draw(minimap);
                        window.draw(map);
                        window.draw(map2);
                        if (!replay_lvl2) {
                            window.draw(shape);
                            window.draw(syntax);
                        }
                        window.display();
                    }
                    ii = 0;
                    heart_num++;
                    health[1].setScale(0.266, 0.266);
                    enemy.setPosition(2023, 850);
                    enemies_anime_d = 0;
                    enemyHealth[1].setScale(0.8, 0.8);
                    if (coins_collected == 0) {
                        bro.setPosition(150, -150);
                        background.setPosition(0, 0);
                    }
                    else if (coins_collected == 1) {
                        bro.setPosition(526, -150);
                        background.setPosition(-4322, 0);
                    }
                    else if (coins_collected == 2) {
                        bro.setPosition(655, -150);
                        background.setPosition(-7100, 0);
                    }
                    else if (coins_collected == 3) {
                        bro.setPosition(718, -150);
                        background.setPosition(-9100, 0);
                    }
                    else if (coins_collected == 4) {
                        bro.setPosition(419, -150);
                        background.setPosition(-11430.6, 0);
                    }
                }
                else {
                    dead = true;
                }
            }
            for (int i = 0; i < heart_num && heart_num > 0; i++) {
                if (timer11 < 0) {
                    heart[i].setTextureRect(IntRect(heart_anime * 125.1, 0, 125.1, 126));
                    if (i == (heart_num - 1)) {
                        heart_anime %= 10;
                        heart_anime++;
                        timer11 = 0.09;
                    }
                    else if (heart_num == 1) {
                        heart_anime++;
                        heart_anime %= 9;
                        timer11 = 0.09;
                    }
                }
                else {
                    if (heart_num > 1)
                        i = (heart_num - 1);
                    timer11 -= deltaTime;
                }
                if (heart_anime == 9) {
                    for (int j = 0; j < heart_num && heart_num > 0; j++) {
                        heart[j].setScale(0, 0);
                    }
                }
            }

            //coins movement
            for (int i = 0; i < 4; i++) {
                if (timer7 < 0) {
                    pyramid.setTextureRect(IntRect(p * 500, 0, 500, 475));
                    coins[i].setTextureRect(IntRect(Cc * 209.7, 0, 209.7, 209));
                    if (i == 3) {
                        p++;
                        p %= 11;
                        Cc++;
                        Cc %= 16;
                        timer7 = 0.03;
                    }
                }
                else {
                    i = 3;
                    timer7 -= deltaTime;
                }
            }

            //coin collect
            if (bro.getGlobalBounds().intersects(coins[0].getGlobalBounds()) && coins_collected < 1) coins_collected = 1;
            else if (bro.getGlobalBounds().intersects(coins[1].getGlobalBounds()) && coins_collected < 2) coins_collected = 2;
            else if (bro.getGlobalBounds().intersects(coins[2].getGlobalBounds())) coins_collected = 3;
            else if (bro.getGlobalBounds().intersects(coins[3].getGlobalBounds())) coins_collected = 4;
            for (int i = 0; i < 4; i++) {
                if (bro.getGlobalBounds().intersects(coins[i].getGlobalBounds())) {
                    if (coin.getStatus() == Music::Stopped) coin.play();
                    coins[i].setScale(0, 0);
                    coin_collect++;
                }
                slot.setTexture(slot_T[coin_collect]);
            }

            //timer display
            if (timer666 < 0) {
                if (TIME_Display > 0) TIME_Display--;
                if (TIME_Display <= 20) {
                    if (end_of_game.getStatus() == Music::Stopped) end_of_game.play();
                    Time.setTexture(timer_T[1]);
                }
                TIME.setString(to_string(TIME_Display));
                timer666 = 3.12;
            }
            else {
                timer666 -= deltaTime;
            }
            if (timer6 < 0) {
                COUNTER += 2.5;
                t++;
                t %= 5;
                Time.setTextureRect(IntRect(t * 490, 0, 490, 348));
                timer6 = 1;
            }
            else {
                timer6 -= deltaTime;
            }

            //gate 
            if (timer2 < 0) {
                gate.setTextureRect(IntRect(i_gate * 195, 0, 195, 137));
                gate2.setTextureRect(IntRect(i_gate * 195, 0, 195, 137));
                i_gate++;
                i_gate %= 6;
                timer2 = 0.15;
            }
            else {
                timer2 -= deltaTime;
            }

            //View's pos
            background_out.setPosition(background.getPosition());
            background_outt.setPosition(background.getPosition().x, background_outt.getPosition().y);
            background_outtt.setPosition(background.getPosition().x, background.getPosition().y);
            neon[0].setPosition(background.getPosition().x + 4490, 400);
            neon[1].setPosition(background.getPosition().x + 4990, 400);
            trap[1].setPosition(background.getPosition().x + 2682, 0);
            trap[2].setPosition(background.getPosition().x + 3182, 866);
            trap[3].setPosition(background.getPosition().x + 3382, 866);
            trap[4].setPosition(background.getPosition().x + 3582, 866);
            trap[5].setPosition(background.getPosition().x + 4500, trap[5].getPosition().y);
            trap[6].setPosition(background.getPosition().x + 5000, trap[6].getPosition().y);
            trap[7].setPosition(background.getPosition().x + 6200, trap[7].getPosition().y);
            trap[8].setPosition(background.getPosition().x + 7257, 835);
            trap[9].setPosition(background.getPosition().x + 9290, trap[9].getPosition().y);
            coins[0].setPosition(background.getPosition().x + 4854, 630);
            coins[1].setPosition(background.getPosition().x + 7762, 699);
            coins[2].setPosition(background.getPosition().x + 9824, 699);
            coins[3].setPosition(background.getPosition().x + 11861, 699);
            cannon.setPosition(background.getPosition().x + 9150, 830);
            cannonHealth[0].setPosition(background.getPosition().x + 9105, 978);
            cannonHealth[1].setPosition(background.getPosition().x + 9105 + 11, 986);
            cannonHealth[2].setPosition(background.getPosition().x + 9105, 978);
            gate.setPosition(background.getPosition().x + 12600, 764);
            gate2.setPosition(background.getPosition().x + 12600, 764);

            //mini map text
            if (bro.getPosition().x < coins[0].getPosition().x) {
                map.setString(to_string((int(coins[0].getPosition().x - bro.getPosition().x)) / 10) + "m");
            }
            else if (bro.getPosition().x > coins[0].getPosition().x && bro.getPosition().x < coins[1].getPosition().x) {
                map.setString(to_string((int(coins[1].getPosition().x - bro.getPosition().x)) / 10) + "m");
            }
            else if (bro.getPosition().x > coins[1].getPosition().x && bro.getPosition().x < coins[2].getPosition().x) {
                map.setString(to_string((int(coins[2].getPosition().x - bro.getPosition().x)) / 10) + "m");
            }
            else if (bro.getPosition().x > coins[2].getPosition().x && bro.getPosition().x < coins[3].getPosition().x) {
                map.setString(to_string((int(coins[3].getPosition().x - bro.getPosition().x)) / 10) + "m");
            }
            map.setPosition(1782, 95);
            map.setOrigin(map.getGlobalBounds().width / 2, map.getGlobalBounds().height / 2);

            if (!replay_lvl2) {
                //syntax
                shape.setSize(Vector2f(syntax.getGlobalBounds().width + 10, syntax.getGlobalBounds().height + 10));
                shape.setOrigin((syntax.getGlobalBounds().width / 2.1) + 5, syntax.getGlobalBounds().height / 1.68);
                shape.setPosition(syntax.getPosition());

                if (COUNTER > 4 && COUNTER <= 8)
                    text = "I'm surprised;";
                else if (COUNTER > 8 && COUNTER <= 12)
                    text = "I didn't think you'd last this long";
                else if (COUNTER > 12 && COUNTER <= 16)
                    text = "well, now you have the ability to sprint and to throw ice balls";
                else if (COUNTER > 16 && COUNTER <= 20)
                    text = "let's see what you can do";
                else if (COUNTER > 20 && COUNTER <= 24)
                    text = "(Stefan) Yeah, we'll see";
                else {
                    text = "";
                    shape.setSize(Vector2f(0, 0));
                }
                syntax.setString(text);
                syntax.setPosition(1920 / 2, 1025);
                syntax.setOrigin(syntax.getGlobalBounds().width / 2.1, syntax.getGlobalBounds().height / 1.68);
            }
            else {
                text = "";
                shape.setSize(Vector2f(0, 0));
                syntax.setString(text);
            }

            //level half ending
            if (bro.getPosition().x > 1558 || level_3) {
                if (gate_sound.getStatus() == Music::Stopped && !level_3) gate_sound.play();
                bro.setScale(0, 0);
                PageNumber = 3;
                //stop all sounds here
                talk.stop();
                foot[0].stop();
                foot[1].stop();
                shoot.stop();
                shoot2.stop();
                die.stop();
                die_cannon.stop();
                jump.stop();
                knife.stop();
                punch.stop();
                hurt.stop();
                coin.stop();
                enemy_sound.stop();
                you_are_dead[0].stop();
                you_are_dead[1].stop();
                end_of_game.stop();
                gameWindow3(window, foot, shoot, jump, punch, hurt, music_bg, game_over_sound, fire_t, health[0], health[1], health[2], slot, Time, TIME, pyramid, TIME_Display, timer_T[1]);
                if (PageNumber != 3) return;
            }
            if (COUNTER > 0) {
                window.clear();
                window.draw(background);
                for (int i = 0; i < 22; i++) {
                    fire[i].setPosition(background.getPosition().x + Fpos, 760);
                    Fpos += 500;
                    window.draw(fire[i]);
                    if (i == 21) Fpos = 500;
                }
                window.draw(background_out);
                for (int i = 2; i < 10; i++) window.draw(trap[i]);
                window.draw(background_outt);
                window.draw(neon[0]);
                window.draw(neon[1]);
                if (COUNTER > 2) {
                    for (int i = 0; i < ammos_right.size(); i++) window.draw(ammos_right[i]);
                    for (int i = 0; i < ammos_left.size(); i++) window.draw(ammos_left[i]);
                    for (int i = 0; i < balls.size(); i++) window.draw(balls[i]);
                }
                for (int i = 0; i < 4; i++) window.draw(coins[i]);
                for (int i = 0; i < 3; i++) window.draw(cannonHealth[i]);
                if (draw_health_enemy) for (int i = 0; i < 3; i++) window.draw(enemyHealth[i]);
                window.draw(enemy);
                window.draw(cannon);
                window.draw(gate);
                window.draw(bro);
                window.draw(background_outtt);
                window.draw(gate2);
                for (int i = 0; i < 2; i++) window.draw(trap[i]);
                window.draw(health[0]);
                window.draw(health[1]);
                window.draw(pyramid);
                for (int i = 0; i < 10; i++) {
                    RECT[i].setSize(Vector2f(trap[i].getGlobalBounds().width - 18, trap[i].getGlobalBounds().height - 18));
                    RECT[i].setPosition(trap[i].getPosition().x + 6, trap[i].getPosition().y);
                    //window.draw(RECT[i]);
                }
                for (int i = 0; i < 3; i++) window.draw(heart[i]);
                window.draw(health[2]);
                window.draw(slot);
                window.draw(Time);
                window.draw(TIME);
                window.draw(minimap);
                window.draw(map);
                window.draw(map2);
                if (!replay_lvl2) {
                    window.draw(shape);
                    window.draw(syntax);
                }
                window.draw(rect);
                window.display();
            }
            deltaTime = c.getElapsedTime().asSeconds();

            if (dead || TIME_Display == 0) {
                music_bg.stop();
                foot[0].stop();
                foot[1].stop();
                shoot.stop();
                shoot2.stop();
                die.stop();
                die_cannon.stop();
                jump.stop();
                knife.stop();
                punch.stop();
                hurt.stop();
                coin.stop();
                enemy_sound.stop();
                you_are_dead[0].stop();
                you_are_dead[1].stop();
                game_over_sound.play();
                end_of_game.stop();
                bro.setTexture(player_dd);
                bro.setPosition(bro.getPosition().x - 70, bro.getPosition().y - 58);
                ii = 4;
                while (ii < 9) {
                    if (timer < 0) {
                        bro.setScale(2, 2);
                        bro.setTextureRect(IntRect(ii * 92, 0, 92, 102));
                        ii++;
                        timer = 0.075;
                    }
                    else {
                        timer -= deltaTime;
                    }
                    window.clear();
                    window.draw(background);
                    for (int i = 0; i < 22; i++) {
                        fire[i].setPosition(background.getPosition().x + Fpos, 760);
                        Fpos += 500;
                        window.draw(fire[i]);
                        if (i == 21) Fpos = 500;
                    }
                    window.draw(background_out);
                    for (int i = 2; i < 10; i++) window.draw(trap[i]);
                    window.draw(background_outt);
                    window.draw(neon[0]);
                    window.draw(neon[1]);
                    for (int i = 0; i < 4; i++) window.draw(coins[i]);
                    for (int i = 0; i < 3; i++) window.draw(cannonHealth[i]);
                    if (draw_health_enemy) for (int i = 0; i < 3; i++) window.draw(enemyHealth[i]);
                    window.draw(enemy);
                    window.draw(cannon);
                    window.draw(gate);
                    window.draw(bro);
                    window.draw(background_outtt);
                    window.draw(gate2);
                    for (int i = 0; i < 2; i++) window.draw(trap[i]);
                    window.draw(health[0]);
                    window.draw(health[1]);
                    window.draw(pyramid);
                    for (int i = 0; i < 3; i++) window.draw(heart[i]);
                    window.draw(health[2]);
                    window.draw(slot);
                    window.draw(Time);
                    window.draw(TIME);
                    window.draw(minimap);
                    window.draw(map);
                    window.draw(map2);
                    if (!replay_lvl2) {
                        window.draw(shape);
                        window.draw(syntax);
                    }
                    window.display();
                }
                PageNumber = -6;
                replay_lvl2 = true;
                gameOver(window, 2);
                if (PageNumber != 2) return;
            }
        }
    }
}

void gameWindow3(RenderWindow& window, Music foot[], Music& shoot, Music& jump, Music& punch, Music& hurt, Music& music_bg, Music& game_over_sound, Texture& fire_t, Sprite& health1, Sprite& health2, Sprite& health3, Sprite& slot, Sprite& Time, Text& TIME, Sprite& pyramid, int& TIME_Display, Texture& timer_T) {
    edit_data();
    int shooting_max_delayb = 20;
    int shooting_max_delay;
    if (hard) shooting_max_delay = 20;
    else shooting_max_delay = 13;

    RectangleShape black_screen(Vector2f(1920, 1080));
    black_screen.setFillColor(Color(0, 0, 0, 190));

    //ground
    RectangleShape rect(Vector2f(2000, 100));
    RectangleShape rect2(Vector2f(2000, 170));
    rect.setPosition(0, 910);
    rect.setFillColor(Color::Transparent);
    rect2.setFillColor(Color::Transparent);

    //background
    Texture b_g;
    b_g.loadFromFile("./assets/textures/map lvl2 #2.png");
    Sprite background;
    background.setTexture(b_g);
    Texture b_gd;
    b_gd.loadFromFile("./assets/textures/map lvl2 ground #2.png");
    Sprite backgroundd;
    backgroundd.setTexture(b_gd);
    Texture b_g_outtt;
    b_g_outtt.loadFromFile("./assets/textures/map lvl2 ground3 #2.png");
    Sprite background_outtt;
    background_outtt.setTexture(b_g_outtt);

    //player
    Texture player_t;
    player_t.loadFromFile("./assets/textures/Hero_P1.png");
    Texture player_d;
    player_d.loadFromFile("./assets/textures/Hero_P2.png");
    Texture player_dd;
    player_dd.loadFromFile("./assets/textures/Sword_sprite.png");
    Sprite bro;
    bro.setTexture(player_t);
    bro.setPosition(150, 230);
    bro.setScale(3, 3);
    Vector2f playerCenter;

    //bats
    Texture bat_T;
    bat_T.loadFromFile("./assets/textures/bat.png");
    Texture bat_d;
    bat_d.loadFromFile("./assets/textures/bat_death.png");
    Sprite bat;
    bat.setTexture(bat_T);
    bat.setPosition(5000, 5000);
    vector<Sprite> bats;
    bats.push_back(Sprite(bat));

    //shoothing
    Texture ammoT;
    ammoT.loadFromFile("./assets/textures/shoot.png");
    Sprite ammo[2];
    for (int i = 0; i < 2; i++) {
        ammo[i].setTexture(ammoT);
        ammo[i].setTextureRect(IntRect(0, 0, 250, 174));
    }
    ammo[0].setScale(0.1, 0.1);
    ammo[1].setScale(-0.1, 0.1);
    vector<Sprite> ammos;
    ammos.push_back(Sprite(ammo[0]));
    vector<float> angles;
    angles.push_back(atan2(Mouse::getPosition(window).y - bro.getPosition().y, Mouse::getPosition(window).x - bro.getPosition().x));

    //fire
    Sprite fire[13];
    Sprite fire2[13];
    for (int i = 0; i < 13; i++) {
        fire[i].setTexture(fire_t);
        fire[i].setScale(0.4, 0.4);
        fire2[i].setScale(0.4, -0.4);
        fire2[i].setTexture(fire_t);
    }

    //gate
    Texture gate_T;
    gate_T.loadFromFile("./assets/textures/gate.png");
    Texture gate2_T;
    gate2_T.loadFromFile("./assets/textures/gate 2.png");
    Sprite gate;
    gate.setTexture(gate_T);
    gate.setScale(1.1, 1.1);
    Sprite gate2;
    gate2.setTexture(gate2_T);
    gate2.setScale(1.1, 1.1);
    Sprite gate_99;
    gate_99.setTexture(gate_T);
    gate_99.setScale(1.1, -1.1);
    Sprite gate2_99;
    gate2_99.setTexture(gate2_T);
    gate2_99.setScale(1.1, -1.1);

    Music bats_sound;
    bats_sound.openFromFile("./assets/sounds/bats.mp3");
    bats_sound.setVolume(50);
    Music gate_sound;
    gate_sound.openFromFile("./assets/sounds/gate.mp3");
    gate_sound.setVolume(17);
    Music end_of_game;
    end_of_game.openFromFile("./assets/sounds/end_of_game (1).mp3");
    end_of_game.setVolume(39);
    Music YOU_WIN;
    YOU_WIN.openFromFile("./assets/sounds/YOU WIN 5.wav");
    if (!hard) shoot.openFromFile("./assets/sounds/shoot 2x.mp3");

    Clock c;
    Vector2f pos_jump(0, 0);
    float deltaTime = 0, timer = 0, timer2 = 0, timer9 = 0, delay = 0.08, gravity = 25, jumphight = 20, timer3 = 0, timer99 = 0, timer6 = 0, timer22 = 0;
    int shoot_delay = 0, shoot_delay2 = 0, shoot_speed = 550, shoot_animation = 0, ii = 0, iv = 0, iii = 0, vii = 0, i_b = 0, i_b99 = 0, speed = 200, f = 0, Fpos = 500, pause_count = 0, p = 0, t = 0, i_gate = 0, COUNTER = 0;
    bool dead = false;
    bool bat_dead = false;
    bool grounded = true;
    char status = 'D';
    while (window.isOpen()) {
        if (PageNumber != 3) break;
        music_bg.setVolume(28 * volume);
        bool play = true;
        c.restart();
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    while (pause_count < 2) {
                        pause_count++;
                        window.draw(black_screen);
                        window.display();
                    }
                    pause_count = 0;
                    music_bg.pause();
                    if (foot[0].getStatus() == Music::Playing) foot[0].pause();
                    if (foot[1].getStatus() == Music::Playing) foot[1].pause();
                    if (shoot.getStatus() == Music::Playing) shoot.pause();
                    if (jump.getStatus() == Music::Playing) jump.pause();
                    if (punch.getStatus() == Music::Playing) punch.pause();
                    if (hurt.getStatus() == Music::Playing) hurt.pause();
                    if (game_over_sound.getStatus() == Music::Playing) game_over_sound.pause();
                    if (bats_sound.getStatus() == Music::Playing) bats_sound.pause();
                    if (end_of_game.getStatus() == Music::Playing) end_of_game.pause();
                    pause(window);
                    if (PageNumber != 3) return;
                    if (sound) music_bg.play();
                    if (foot[0].getStatus() == Music::Paused) foot[0].play();
                    if (foot[1].getStatus() == Music::Paused) foot[1].play();
                    if (shoot.getStatus() == Music::Paused) shoot.play();
                    if (jump.getStatus() == Music::Paused) jump.play();
                    if (punch.getStatus() == Music::Paused) punch.play();
                    if (hurt.getStatus() == Music::Paused) hurt.play();
                    if (game_over_sound.getStatus() == Music::Paused) game_over_sound.play();
                    if (bats_sound.getStatus() == Music::Paused) bats_sound.play();
                    if (end_of_game.getStatus() == Music::Paused) end_of_game.play();
                    play = false;
                    break;
                }
            }
        }
        if (play) {
            if (bro.getPosition().x < 300 && background.getPosition().x < 300 && background.getPosition().x > 0) background.setPosition(0, 0);

            //player movement
            bool AFK = true;
            if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::LShift) && grounded) {
                shoot.stop();
                if (foot[1].getStatus() == Music::Stopped) foot[1].play();
                foot[0].stop();
                if (timer < 0) {
                    bro.setTextureRect(IntRect(vii * 29, 283, 29, 30));
                    vii++;
                    vii %= 8;
                    timer = delay;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'D';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    background.move(-speed * 2 * deltaTime, 0);
                    for (int i = 0; i < bats.size(); i++) {
                        bats[i].move(-speed * 2 * deltaTime, 0);
                    }
                    for (int i = 0; i < ammos.size(); i++) {
                        ammos[i].move(-speed * 2 * deltaTime, 0);
                    }
                    bro.move(deltaTime * 20, 0);
                }
                else {
                    bro.move(speed * 2 * deltaTime, 0);
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::LShift) && grounded) {
                shoot.stop();
                if (foot[1].getStatus() == Music::Stopped) foot[1].play();
                foot[0].stop();
                if (timer < 0) {
                    bro.setTextureRect(IntRect(vii * 29, 313, 29, 30));
                    vii++;
                    vii %= 8;
                    timer = delay;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'A';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    background.move(speed * 2 * deltaTime, 0);

                    for (int i = 0; i < ammos.size(); i++) {
                        ammos[i].move(speed * 2 * deltaTime, 0);
                    }
                    bro.move(-deltaTime * 20, 0);
                }
                else {
                    bro.move(-speed * 2 * deltaTime, 0);
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::D) && Mouse::isButtonPressed(Mouse::Left)) {
                if (shoot.getStatus() == Music::Stopped) shoot.play();
                if (grounded) {
                    if (foot[0].getStatus() == Music::Stopped) foot[0].play();
                    foot[1].stop();
                }
                else {
                    foot[0].stop();
                    foot[1].stop();
                }
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iv * 22, 343, 22, 32));
                    iv++;
                    iv %= 6;
                    timer = delay;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'D';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    background.move(-speed * deltaTime, 0);
                    for (int i = 0; i < bats.size(); i++) {
                        bats[i].move(-speed * deltaTime, 0);
                    }
                    for (int i = 0; i < ammos.size(); i++) {
                        ammos[i].move(-speed * deltaTime, 0);
                    }
                    bro.move(deltaTime * 20, 0);
                }
                else {
                    bro.move(speed * deltaTime, 0);
                }
                //shooting D
                if (shoot_delay > shooting_max_delay) {
                    shoot_delay = 0;
                    ammo[0].setPosition(playerCenter);
                    ammos.push_back(Sprite(ammo[0]));
                    angles.push_back(atan2(Mouse::getPosition(window).y - 51 - bro.getPosition().y, Mouse::getPosition(window).x - bro.getPosition().x));
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::A) && Mouse::isButtonPressed(Mouse::Left)) {
                if (shoot.getStatus() == Music::Stopped) shoot.play();
                if (grounded) {
                    if (foot[0].getStatus() == Music::Stopped) foot[0].play();
                    foot[1].stop();
                }
                else {
                    foot[0].stop();
                    foot[1].stop();
                }
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iv * 22, 375, 22, 32));
                    iv++;
                    iv %= 6;
                    timer = delay;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'A';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    background.move(speed * deltaTime, 0);

                    for (int i = 0; i < ammos.size(); i++) {
                        ammos[i].move(speed * deltaTime, 0);
                    }
                    bro.move(-deltaTime * 20, 0);
                }
                else {
                    bro.move(-speed * deltaTime, 0);
                }
                //shooting A
                if (shoot_delay > shooting_max_delay) {
                    shoot_delay = 0;
                    ammo[1].setPosition(playerCenter);
                    ammos.push_back(Sprite(ammo[1]));
                    angles.push_back(atan2(Mouse::getPosition(window).y - 51 - bro.getPosition().y, Mouse::getPosition(window).x - bro.getPosition().x));
                }
            }
            else if (status == 'D' && Mouse::isButtonPressed(Mouse::Left)) {
                if (shoot.getStatus() == Music::Stopped) shoot.play();
                foot[0].stop();
                foot[1].stop();
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iv * 22, 407, 22, 33));
                    iv++;
                    iv %= 6;
                    timer = delay;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'D';
                AFK = false;
                //shooting D (not moving)
                if (shoot_delay > shooting_max_delay) {
                    shoot_delay = 0;
                    ammo[0].setPosition(playerCenter);
                    ammos.push_back(Sprite(ammo[0]));
                    angles.push_back(atan2(Mouse::getPosition(window).y - 51 - bro.getPosition().y, Mouse::getPosition(window).x - bro.getPosition().x));
                }
            }
            else if (status == 'A' && Mouse::isButtonPressed(Mouse::Left)) {
                if (shoot.getStatus() == Music::Stopped) shoot.play();
                foot[0].stop();
                foot[1].stop();
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iv * 22, 440, 22, 33));
                    iv++;
                    iv %= 6;
                    timer = delay;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'A';
                AFK = false;
                //shooting A (not moving)
                if (shoot_delay > shooting_max_delay) {
                    shoot_delay = 0;
                    ammo[1].setPosition(playerCenter);
                    ammos.push_back(Sprite(ammo[1]));
                    angles.push_back(atan2(Mouse::getPosition(window).y - 51 - bro.getPosition().y, Mouse::getPosition(window).x - bro.getPosition().x));
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::D)) {
                shoot.stop();
                if (grounded) {
                    if (foot[0].getStatus() == Music::Stopped) foot[0].play();
                    foot[1].stop();
                }
                else {
                    foot[0].stop();
                    foot[1].stop();
                }
                if (timer < 0) {
                    bro.setTextureRect(IntRect(ii * 20, 175, 20, 35));
                    ii++;
                    ii %= 8;
                    timer = delay;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'D';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    background.move(-speed * deltaTime, 0);
                    for (int i = 0; i < bats.size(); i++) {
                        bats[i].move(-speed * deltaTime, 0);
                    }
                    for (int i = 0; i < ammos.size(); i++) {
                        ammos[i].move(-speed * deltaTime, 0);
                    }
                    bro.move(deltaTime * 20, 0);
                }
                else {
                    bro.move(speed * deltaTime, 0);
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::A)) {
                shoot.stop();
                if (grounded) {
                    if (foot[0].getStatus() == Music::Stopped) foot[0].play();
                    foot[1].stop();
                }
                else {
                    foot[0].stop();
                    foot[1].stop();
                }
                if (timer < 0) {
                    bro.setTextureRect(IntRect(ii * 20, 248, 20, 35));
                    ii++;
                    ii %= 8;
                    timer = delay;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'A';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    background.move(speed * deltaTime, 0);

                    for (int i = 0; i < ammos.size(); i++) {
                        ammos[i].move(speed * deltaTime, 0);
                    }
                    bro.move(-deltaTime * 20, 0);
                }
                else {
                    bro.move(-speed * deltaTime, 0);
                }
            }
            else {
                shoot.stop();
                foot[0].stop();
                foot[1].stop();
            }

            if (Mouse::isButtonPressed(Mouse::Left) && (Mouse::getPosition(window).x - bro.getPosition().x > 0))
                status = 'D';
            else if (Mouse::isButtonPressed(Mouse::Left) && bro.getPosition().x - Mouse::getPosition(window).x > 0)
                status = 'A';

            //player AFK movement
            if (status == 'D' && AFK) {
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iii * 19, 35, 19, 35));
                    iii++;
                    iii %= 8;
                    timer = delay;
                }
                else {
                    timer -= deltaTime;
                }
            }
            else if (status == 'A' && AFK) {
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iii * 19, 105, 19, 35));
                    iii++;
                    iii %= 8;
                    timer = delay;
                }
                else {
                    timer -= deltaTime;
                }
            }

            //player jump & gravity
            if ((bro.getGlobalBounds().intersects(rect.getGlobalBounds()))) {
                speed = 200;
                pos_jump.y = 0;
                grounded = true;
            }
            else if ((bro.getGlobalBounds().intersects(rect2.getGlobalBounds()))) {
                speed = 200;
                pos_jump.y = 0;
                grounded = true;
            }
            else {
                speed = 330;
                pos_jump.y += gravity * deltaTime;
            }

            if (Keyboard::isKeyPressed(Keyboard::Space) && grounded) {
                jump.play();
                pos_jump.y -= jumphight;
                grounded = false;
                gravity = -gravity;
                jumphight = -jumphight;
                bro.setScale(bro.getScale().x, -bro.getScale().y);
            }
            bro.move(pos_jump);

            //fire movement
            for (int i = 0; i < 13; i++) {
                if (timer3 < 0) {
                    fire[i].setTextureRect(IntRect(f * 167, 0, 167, 342));
                    fire2[i].setTextureRect(IntRect(f * 167, 0, 167, 342));
                    if (i == 12) {
                        f++;
                        f %= 7;
                        timer3 = 0.15;
                    }
                }
                else {
                    i = 12;
                    timer3 -= deltaTime;
                }
            }

            //bat movement
            shoot_delay2++;
            for (int i = 0; i < bats.size(); i++) {
                if (timer2 < 0) {
                    if (bats[i].getTexture() == &bat_T) {
                        bats[i].setTextureRect(IntRect(i_b * 91, 0, 91, 114));
                    }
                    pyramid.setTextureRect(IntRect(p * 500, 0, 500, 475));
                    if (i == bats.size() - 1) {
                        p++;
                        p %= 11;
                        i_b++;
                        i_b %= 9;
                        timer2 = 0.12;
                    }
                }
                else {
                    i = bats.size() - 1;
                    timer2 -= deltaTime;
                }
            }
            if (bat_dead) {
                for (int i = 0; i < bats.size(); i++) {
                    if (i_b99 != 5) {
                        if (timer99 < 0) {
                            if (bats[i].getTexture() == &bat_d) {
                                bats[i].setTextureRect(IntRect(i_b99 * 49, 0, 49, 48));
                                bats[i].setScale(2, 2);
                            }
                            if (i == bats.size() - 1) {
                                i_b99++;
                                i_b99 %= 6;
                                timer99 = 0.09;
                            }
                        }
                        else {
                            i = bats.size() - 1;
                            timer99 -= deltaTime;
                        }
                    }
                    else {
                        if (bats[i].getTexture() == &bat_d) {
                            bats.erase(bats.begin() + i);
                        }
                    }
                }

            }
            for (int i = 0; i < bats.size(); i++) {
                bats[i].move(-shoot_speed * deltaTime, 0);

                if (bats[i].getPosition().x < -100) {
                    bats.erase(bats.begin() + i);
                }
            }
            if (shoot_delay2 > (shooting_max_delayb * 0.8) && (background.getPosition().x > -11200)) {
                shoot_delay2 = 0;
                bat.setPosition(1920, 215 + (rand() % 536));
                bats.push_back(Sprite(bat));
            }
            for (int i = 0; i < ammos.size(); i++) {
                for (int j = 0; j < bats.size(); j++) {
                    if (ammos[i].getGlobalBounds().intersects(bats[j].getGlobalBounds()) && bats[j].getTexture() == &bat_T) {
                        if (punch.getStatus() == Music::Stopped) punch.play();
                        bats[j].setTexture(bat_d);
                        bat_dead = true;
                        i_b99 = 0;
                        ammos[i].setScale(0, 0);
                    }
                }
            }
            for (int j = 0; j < bats.size(); j++) {
                if (bro.getGlobalBounds().intersects(bats[j].getGlobalBounds()) && bats[j].getTexture() == &bat_T)
                    dead = true;
            }
            if (bats.size() == 0) {
                bats_sound.stop();
            }
            else {
                if (bats_sound.getStatus() == Music::Stopped) {
                    bats_sound.play();
                }
            }

            //shooting
            shoot_delay++;
            if (gravity < 0) {
                playerCenter = Vector2f(bro.getPosition().x + (bro.getGlobalBounds().width / 2), (bro.getPosition().y + (bro.getGlobalBounds().height / 2)) - 100);
            }
            else {
                playerCenter = Vector2f(bro.getPosition().x + (bro.getGlobalBounds().width / 2), bro.getPosition().y + (bro.getGlobalBounds().height / 2));
            }
            for (int i = 0; i < ammos.size(); i++) {
                if (timer9 < 0) {
                    ammos[i].setTextureRect(IntRect(shoot_animation * 250, 0, 250, 174));
                    if (i == ammos.size() - 1) {
                        shoot_animation++;
                        shoot_animation %= 4;
                        timer9 = 0.15;
                    }
                }
                else {
                    i = ammos.size() - 1;
                    timer9 -= deltaTime;
                }
            }
            for (int i = 0; i < ammos.size(); i++) {
                ammos[i].move(shoot_speed * cos(angles[i]) * deltaTime, shoot_speed * sin(angles[i]) * deltaTime);

                if (abs(ammos[i].getPosition().x - bro.getPosition().x) > 600 || abs(ammos[i].getPosition().y - bro.getPosition().y) > 400) {
                    ammos.erase(ammos.begin() + i);
                    angles.erase(angles.begin() + i);
                }
            }

            //timer display
            if (timer6 < 0) {
                COUNTER++;
                if (TIME_Display > 0) TIME_Display--;
                if (TIME_Display <= 20) {
                    if (end_of_game.getStatus() == Music::Stopped) end_of_game.play();
                    Time.setTexture(timer_T);
                }
                t++;
                t %= 5;
                Time.setTextureRect(IntRect(t * 490, 0, 490, 348));
                TIME.setString(to_string(TIME_Display));
                timer6 = 1;
            }
            else {
                timer6 -= deltaTime;
            }

            //gate
            if (timer22 < 0) {
                gate.setTextureRect(IntRect(i_gate * 195, 0, 195, 137));
                gate2.setTextureRect(IntRect(i_gate * 195, 0, 195, 137));
                gate_99.setTextureRect(IntRect(i_gate * 195, 0, 195, 137));
                gate2_99.setTextureRect(IntRect(i_gate * 195, 0, 195, 137));
                i_gate++;
                i_gate %= 6;
                timer22 = 0.15;
            }
            else {
                timer22 -= deltaTime;
            }

            //level ending
            if (bro.getPosition().x > 1558) {
                if (gate_sound.getStatus() == Music::Stopped) gate_sound.play();
                bro.setScale(0, 0);
                PageNumber = -7;
                //stop all sounds here
                YOU_WIN.play();
                bats_sound.stop();
                music_bg.stop();
                foot[0].stop();
                foot[1].stop();
                shoot.stop();
                jump.stop();
                punch.stop();
                hurt.stop();
                game_over_sound.stop();
                end_of_game.stop();
                winWindow(window, 4);
                if (PageNumber != 4) return;
                gameWindow4(window, slot);
                if (PageNumber != 4) return;
            }

            backgroundd.setPosition(background.getPosition().x, 0);
            background_outtt.setPosition(background.getPosition().x, background.getPosition().y);
            gate.setPosition(background.getPosition().x + 12600, 764);
            gate2.setPosition(background.getPosition().x + 12600, 764);
            gate_99.setPosition(background.getPosition().x + 12600, 316);
            gate2_99.setPosition(background.getPosition().x + 12600, 316);
            if (COUNTER > 0) {
                window.clear();
                window.draw(background);
                for (int i = 0; i < 13; i++) {
                    fire[i].setPosition(background.getPosition().x + Fpos, 760);
                    fire2[i].setPosition(background.getPosition().x + Fpos - 500, 320);
                    Fpos += 1000;
                    window.draw(fire[i]);
                    window.draw(fire2[i]);
                    if (i == 12) Fpos = 500;
                }
                window.draw(backgroundd);
                window.draw(gate);
                window.draw(gate_99);
                window.draw(bro);
                window.draw(background_outtt);
                window.draw(gate2);
                window.draw(gate2_99);
                for (int i = 0; i < bats.size(); i++) window.draw(bats[i]);
                for (int i = 0; i < ammos.size(); i++) window.draw(ammos[i]);
                window.draw(health1);
                window.draw(health2);
                window.draw(pyramid);
                window.draw(health3);
                window.draw(slot);
                window.draw(Time);
                window.draw(TIME);
                window.display();
            }
            deltaTime = c.getElapsedTime().asSeconds();
            if (dead || TIME_Display == 0) {
                health2.setScale(0, 0);
                music_bg.stop();
                bats_sound.stop();
                foot[0].stop();
                foot[1].stop();
                shoot.stop();
                jump.stop();
                punch.stop();
                hurt.stop();
                game_over_sound.play();
                end_of_game.stop();
                ii = 4;
                while (ii < 9) {
                    if (timer < 0) {
                        bro.setTexture(player_dd);
                        bro.setScale(2, 2);
                        bro.setTextureRect(IntRect(ii * 92, 0, 92, 102));
                        bro.setOrigin(bro.getTextureRect().width / 3, bro.getTextureRect().height);
                        ii++;
                        timer = 0.07;
                    }
                    else {
                        timer -= deltaTime;
                    }
                    window.clear();
                    window.draw(background);
                    for (int i = 0; i < 13; i++) {
                        fire[i].setPosition(background.getPosition().x + Fpos, 760);
                        fire2[i].setPosition(background.getPosition().x + Fpos - 500, 320);
                        Fpos += 1000;
                        window.draw(fire[i]);
                        window.draw(fire2[i]);
                        if (i == 12) Fpos = 500;
                    }
                    window.draw(backgroundd);
                    window.draw(gate);
                    window.draw(gate_99);
                    window.draw(bro);
                    window.draw(background_outtt);
                    window.draw(gate2);
                    window.draw(gate2_99);
                    for (int i = 0; i < bats.size(); i++) window.draw(bats[i]);
                    window.draw(health1);
                    window.draw(health2);
                    window.draw(pyramid);
                    window.draw(health3);
                    window.draw(slot);
                    window.draw(Time);
                    window.draw(TIME);
                    window.draw(rect);
                    window.draw(rect2);
                    window.display();
                }
                PageNumber = -6;
                gameOver(window, 3);
                if (PageNumber != 3) return;
            }
        }
    }
}

void gameWindow4(RenderWindow& window, Sprite& slot) {
    edit_data();
    RectangleShape rect(Vector2f(2000, 100));
    rect.setPosition(0, 910);
    rect.setFillColor(Color::Transparent);

    RectangleShape black_screen(Vector2f(1920, 1080));
    black_screen.setFillColor(Color(0, 0, 0, 190));

    //hitbox
    RectangleShape HitBox;
    HitBox.setFillColor(Color::Transparent);
    HitBox.setOutlineThickness(2);
    HitBox.setOutlineColor(Color::Red);

    //background
    Texture background_T;
    background_T.loadFromFile("./assets/textures/map lvl3 background.png");
    Sprite background;
    background.setTexture(background_T);
    background.setPosition(0, -14920);
    Texture background_out_T;
    background_out_T.loadFromFile("./assets/textures/map lvl3 ground.png");
    Sprite background_out;
    background_out.setTexture(background_out_T);
    background_out.setPosition(0, -14920);
    Vector2f bg_pos;
    RectangleShape walls[2];
    walls[1].setPosition(1920 - 206, 0);
    for (int i = 0; i < 2; i++)
        walls[i].setSize(Vector2f(207, 1080));

    //player
    Texture player_t;
    player_t.loadFromFile("./assets/textures/Hero_P1.png");
    //Texture player_d;
    //player_d.loadFromFile("./assets/textures/Hero_P2.png");
    Sprite bro;
    bro.setTexture(player_t);
    bro.setPosition(350, 822);
    Vector2f pos_right_D;
    Vector2f pos_left_A;

    //player health
    Texture health_T[3];
    health_T[0].loadFromFile("./assets/textures/health.png");
    health_T[1].loadFromFile("./assets/textures/health2.png");
    health_T[2].loadFromFile("./assets/textures/health3.png");
    Sprite health[3];
    health[1].setPosition(141, 116);
    for (int i = 0; i < 3; i++) {
        health[i].setTexture(health_T[i]);
        health[i].setPosition(health[i].getPosition().x + 38, health[i].getPosition().y + 10);
        health[i].setScale(0.266, 0.266);
    }
    Texture heart_T;
    heart_T.loadFromFile("./assets/textures/heart.png");
    Sprite heart[3];
    for (int i = 0; i < 3; i++) {
        heart[i].setTexture(heart_T);
        heart[i].setTextureRect(IntRect(0, 0, 125.1, 126));
        heart[i].setScale(0.3, 0.3);
    }
    heart[2].setPosition(48.55, 125.5);
    heart[1].setPosition(48.55 + (126 * 0.3), 125.5);
    heart[0].setPosition(48.55 + (252 * 0.3), 125.5);

    //timer
    Texture timer_T[2];
    timer_T[0].loadFromFile("./assets/textures/timer.png");
    timer_T[1].loadFromFile("./assets/textures/timer_end.png");
    Sprite Time;
    Time.setTexture(timer_T[0]);
    Time.setScale(0.266, 0.266);
    Time.setTextureRect(IntRect(0, 0, 490, 348));
    Time.setPosition((1920 / 2) - (Time.getGlobalBounds().width / 2), Time.getPosition().y + 10);

    //timer Text
    Font font;
    font.loadFromFile("./assets/font/trebuc.ttf");
    Text TIME;
    TIME.setFont(font);
    TIME.setCharacterSize(50);
    TIME.setFillColor(Color::Black);
    TIME.setPosition((1920 / 2) - (Time.getGlobalBounds().width / 2) + 16, Time.getPosition().y + 7);

    //pyramid
    Texture pyramid_T;
    pyramid_T.loadFromFile("./assets/textures/pyramid.png");
    Sprite pyramid;
    pyramid.setTexture(pyramid_T);
    pyramid.setPosition(50, 20);
    pyramid.setScale(0.22, 0.3);

    //fire
    Texture fire_t;
    fire_t.loadFromFile("./assets/textures/torch2.png");
    Sprite fire[11];
    Sprite fire2[11];
    for (int i = 0; i < 10; i++) {
        fire[i].setTexture(fire_t);
        fire[i].setScale(0.5, 0.5);
        fire[i].setRotation(38);
        fire2[i].setTexture(fire_t);
        fire2[i].setScale(-0.5, 0.5);
        fire2[i].setRotation(-38);
    }
    fire[10].setTexture(fire_t);
    fire[10].setScale(0.5, 0.5);
    fire2[10].setTexture(fire_t);
    fire2[10].setScale(0.5, 0.5);

    //syntax
    String text;
    Text syntax;
    syntax.setFont(font);
    syntax.setCharacterSize(25);
    RectangleShape shape99;
    shape99.setFillColor(Color(0, 0, 0, 140));
    Music lvl3;
    lvl3.openFromFile("./assets/sounds/easy-hard-mode-lvl3.mp3");
    Music gate_sound;
    gate_sound.openFromFile("./assets/sounds/gate.mp3");
    gate_sound.setVolume(17);
    if (!replay_lvl3) lvl3.play();


    //wood
    Texture wood_t;
    wood_t.loadFromFile("./assets/textures/wood.png");
    Texture bigWood_t;
    bigWood_t.loadFromFile("./assets/textures/large wood.png");
    Sprite wood[3];
    int height_deff = -260;
    RectangleShape collision_shapes[4];
    for (int i = 0; i < 3; i++) {
        collision_shapes[i].setSize(Vector2f(403, 30));
        collision_shapes[i].setFillColor(Color(240, 240, 240, 200));
        wood[i].setTexture(wood_t);
        wood[i].setPosition((210 + (rand() % 1118)), height_deff);
        height_deff += 390;
    }
    collision_shapes[3].setSize(Vector2f(1920, 30));
    collision_shapes[3].setPosition(-5000, -5000);
    Sprite base_wood;
    base_wood.setTexture(bigWood_t);

    Music foot;
    foot.openFromFile("./assets/sounds/foot.mp3");
    foot.setVolume(34);
    foot.setLoop(true);
    Music jump;
    jump.openFromFile("./assets/sounds/jump.mp3");
    jump.setVolume(69);
    Music YOU_WIN;
    YOU_WIN.openFromFile("./assets/sounds/YOU WIN 5.wav");
    Music music_bg;
    music_bg.openFromFile("./assets/sounds/bg_music_FULL.mp3");
    music_bg.setVolume(28 * volume);
    music_bg.setLoop(true);
    Music game_over_sound;
    game_over_sound.openFromFile("./assets/sounds/game_over_sound.mp3");
    Music end_of_game;
    end_of_game.openFromFile("./assets/sounds/end_of_game (1).mp3");
    end_of_game.setVolume(39);

    Clock c;
    Vector2f pos_jump(0, 0);
    float deltaTime = 0, timer3 = 0, timer = 0, gravity = 60, jumphight = 25, timer7 = 0, timer6 = 0, COUNTER = 0, COUNTER2 = 0, TIME_R = 0;
    int f = 0, Fpos = 0, speed = 500, ii = 0, iii = 0, bg_speed = 250, jump_delay = 0, p = 0, TIME_Display = 100, t = 0, pause_count = 0;
    bool grounded = true;
    bool WIN = false;
    bool Bool = false;
    char status = 'D';
    if (sound) music_bg.play();
    while (window.isOpen()) {
        if (PageNumber != 4) break;
        c.restart();
        bool play = true;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape && !WIN) {
                    while (pause_count < 2) {
                        pause_count++;
                        window.draw(black_screen);
                        window.display();
                    }
                    pause_count = 0;
                    music_bg.pause();
                    if (foot.getStatus() == Music::Playing) foot.pause();
                    if (jump.getStatus() == Music::Playing) jump.pause();
                    if (lvl3.getStatus() == Music::Playing) lvl3.pause();
                    if (end_of_game.getStatus() == Music::Playing) end_of_game.pause();
                    pause(window);
                    if (PageNumber != 4) return;
                    if (sound) music_bg.play();
                    if (foot.getStatus() == Music::Paused) foot.play();
                    if (jump.getStatus() == Music::Paused) jump.play();
                    if (lvl3.getStatus() == Music::Paused) lvl3.play();
                    if (end_of_game.getStatus() == Music::Paused) end_of_game.play();
                    play = false;
                    break;
                }
            }
        }
        if (play) {
            if (WIN) {
                foot.stop();
                jump.stop();
                lvl3.stop();
                music_bg.stop();
                end_of_game.stop();
                if (YOU_WIN.getStatus() == Music::Stopped) {
                    YOU_WIN.play();
                    gate_sound.play();
                }
                if (COUNTER2 > 3.3) {
                    PageNumber = -7;
                    winWindow(window, 5);
                    if (PageNumber != 5) return;
                }
            }

            //background movement
            bg_pos.y = bg_speed * deltaTime;
            if ((COUNTER > 33.5 && background.getPosition().y <= -1 && !replay_lvl3) || (COUNTER > 8.2 && background.getPosition().y <= -1 && replay_lvl3)) { // IF SOUND STOPED 
                background.move(bg_pos);
                if (rect.getPosition().y > 1080) {
                    bro.move(bg_pos);
                }
                for (int i = 0; i < 3; i++) wood[i].move(bg_pos);
                background_out.move(0, bg_pos.y * 0.75);
                rect.move(0, bg_pos.y * 0.75);
            }
            else if (COUNTER > 10 && base_wood.getGlobalBounds().intersects(bro.getGlobalBounds())) {
                WIN = true;
            }
            else
                WIN = false;

            //background speed
            if (hard) {
                if (int(COUNTER) == 82)
                    bg_speed = 400;
                else if (int(COUNTER) == 164)
                    bg_speed = 600;
            }
            else {
                if (int(COUNTER) == 82)
                    bg_speed = 350;
                else if (int(COUNTER) == 164)
                    bg_speed = 450;
            }
            //player animation
            bool AFK = true;
            bro.setScale(3, 3);
            bro.setOrigin(0, bro.getTextureRect().height);
            if (Keyboard::isKeyPressed(Keyboard::D)) {
                if (grounded || Bool) {
                    if (foot.getStatus() == Music::Stopped) foot.play();
                }
                else {
                    foot.stop();
                }
                pos_left_A.x = 0;
                if (timer < 0) {
                    bro.setTextureRect(IntRect(ii * 20, 175, 20, 35));
                    ii++;
                    ii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'D';
                AFK = false;
                pos_right_D.x = speed * deltaTime;
            }
            else if (Keyboard::isKeyPressed(Keyboard::A)) {
                if (grounded || Bool) {
                    if (foot.getStatus() == Music::Stopped) foot.play();
                }
                else {
                    foot.stop();
                }
                pos_right_D.x = 0;
                if (timer < 0) {
                    bro.setTextureRect(IntRect(ii * 20, 248, 20, 35));
                    ii++;
                    ii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'A';
                AFK = false;
                pos_left_A.x = -speed * deltaTime;
            }
            else {
                foot.stop();
                pos_right_D.x = 0;
                pos_left_A.x = 0;
            }
            bro.move(pos_right_D);
            bro.move(pos_left_A);

            //player AFK movement
            if (status == 'D' && AFK) {
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iii * 19, 35, 19, 35));
                    iii++;
                    iii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
            }
            else if (status == 'A' && AFK) {
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iii * 19, 105, 19, 35));
                    iii++;
                    iii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
            }
            HitBox.setOrigin(0, bro.getGlobalBounds().height);
            HitBox.setSize(Vector2f(bro.getGlobalBounds().width, bro.getGlobalBounds().height + 22));
            HitBox.setPosition(bro.getPosition());

            //player jump & gravity
            if ((bro.getGlobalBounds().intersects(rect.getGlobalBounds()))) {
                speed = 488;
                pos_jump.y = 0;
                grounded = true;
            }
            else if (jump_delay > 10 && Bool) {
                speed = 488;
                pos_jump.y = 0;
                grounded = true;
            }
            else {
                if (COUNTER < 20)
                    speed = 788;
                else if (COUNTER >= 20 && COUNTER < 40) {
                    speed = 988;
                    jumphight = 26.5;
                }
                else if (COUNTER >= 40) {
                    speed = 1222;
                    jumphight = 28.5;
                }
                pos_jump.y += gravity * deltaTime;
                grounded = false;
            }

            if (Keyboard::isKeyPressed(Keyboard::Space) && grounded) {
                jump.play();
                pos_jump.y -= jumphight;
                grounded = false;
                jump_delay = 0;
            }
            bro.move(pos_jump);

            //fire movement
            for (int i = 0; i < 11; i++) {
                if (timer3 < 0) {
                    fire[i].setTextureRect(IntRect(f * 167, 0, 167, 342));
                    fire2[i].setTextureRect(IntRect(f * 167, 0, 167, 342));
                    if (i == 10) {
                        f++;
                        f %= 7;
                        timer3 = 0.15;
                    }
                }
                else {
                    i = 10;
                    timer3 -= deltaTime;
                }
            }

            //pyramid
            if (timer7 < 0) {
                pyramid.setTextureRect(IntRect(p * 500, 0, 500, 475));
                p++;
                p %= 11;
                timer7 = 0.03;
            }
            else {
                timer7 -= deltaTime;
            }

            //timer display
            if (timer6 < 0) {
                if (TIME_Display > 0) TIME_Display--;
                if (TIME_Display <= 20) {
                    Time.setTexture(timer_T[1]);
                    if (end_of_game.getStatus() == Music::Stopped) end_of_game.play();
                }
                t += 2.5;
                t %= 5;
                Time.setTextureRect(IntRect(t * 490, 0, 490, 348));
                TIME.setString(to_string(TIME_Display));
                timer6 = 1;
            }
            else {
                timer6 -= deltaTime;
            }
            if (TIME_R < 0) {
                COUNTER += 0.5;
                if (WIN) COUNTER2 += 0.5;
                TIME_R = 0.1;
            }
            else {
                TIME_R -= deltaTime;
            }

            //wood
            for (int i = 0; i < 3; i++) {
                collision_shapes[i].setPosition(wood[i].getPosition() + Vector2f(15, 29));
                if (wood[i].getPosition().y > 1080 && background.getPosition().y < -640) {
                    wood[i].setPosition((210 + (rand() % 1118)), -50);
                }
            }

            //collision
            FloatRect broBounds = bro.getGlobalBounds();
            Bool = false;
            if (COUNTER > 6) {
                for (int i = 0; i < 4; i++) {
                    if (broBounds.intersects(collision_shapes[i].getGlobalBounds())) {
                        Bool = true;
                        Vector2f correctionVector;
                        float xOverlap = min(broBounds.left + broBounds.width - collision_shapes[i].getGlobalBounds().left,
                            collision_shapes[i].getGlobalBounds().left + collision_shapes[i].getGlobalBounds().width - broBounds.left);
                        float yOverlap = min(broBounds.top + broBounds.height - collision_shapes[i].getGlobalBounds().top,
                            collision_shapes[i].getGlobalBounds().top + collision_shapes[i].getGlobalBounds().height - broBounds.top);
                        if (xOverlap > yOverlap) {
                            correctionVector.y = (broBounds.top < collision_shapes[i].getGlobalBounds().top) ? -yOverlap : 0;
                        }
                        bro.move(correctionVector);
                    }
                }
                for (int i = 0; i < 2; i++) {
                    if (broBounds.intersects(walls[i].getGlobalBounds())) {
                        Vector2f correctionVector;
                        float xOverlap = min(broBounds.left + broBounds.width - walls[i].getGlobalBounds().left,
                            walls[i].getGlobalBounds().left + walls[i].getGlobalBounds().width - broBounds.left);
                        float yOverlap = min(broBounds.top + broBounds.height - walls[i].getGlobalBounds().top,
                            walls[i].getGlobalBounds().top + walls[i].getGlobalBounds().height - broBounds.top);
                        if (xOverlap < yOverlap) {
                            correctionVector.x = (broBounds.left < walls[i].getGlobalBounds().left) ? -xOverlap : xOverlap;
                        }
                        bro.move(correctionVector);
                    }
                }

                for (int i = 2; i >= 0; i--) {
                    if (HitBox.getGlobalBounds().intersects(collision_shapes[i].getGlobalBounds())) {
                        jump_delay++;
                        break;
                    }
                }
            }

            //syntax
            shape99.setSize(Vector2f(syntax.getGlobalBounds().width + 10, syntax.getGlobalBounds().height + 10));
            shape99.setOrigin((syntax.getGlobalBounds().width / 2.1) + 5, syntax.getGlobalBounds().height / 1.68);
            shape99.setPosition(syntax.getPosition());

            if (!replay_lvl1) {
                if (COUNTER > 3 && COUNTER <= 13)
                    text = "To make it to the top";
                else if (COUNTER > 13 && COUNTER <= 23)
                    text = "jump on the wooden steps,";
                else if (COUNTER > 23 && COUNTER <= 33)
                    text = "but watch out as the speed increases with time";
                else {
                    text = "";
                    shape99.setSize(Vector2f(0, 0));
                }
            }
            else {
                text = "";
                shape99.setSize(Vector2f(0, 0));
            }
            syntax.setString(text);
            syntax.setPosition(1920 / 2, 1025);
            syntax.setOrigin(syntax.getGlobalBounds().width / 2.1, syntax.getGlobalBounds().height / 1.68);


            if (bro.getPosition().y > 1200 || TIME_Display == 0) { //DEAD
                music_bg.stop();
                jump.stop();
                foot.stop();
                end_of_game.stop();
                game_over_sound.play();
                sleep(milliseconds(200));
                PageNumber = -6;
                replay_lvl3 = true;
                gameOver(window, 4);
                if (PageNumber != 4) return;
            }
            if (COUNTER > 0) {
                window.clear();
                window.draw(background);
                for (int i = 0; i < 3; i++) {
                    window.draw(wood[i]);
                    //window.draw(collision_shapes[i]);
                }
                fire[10].setPosition(200, background_out.getPosition().y + 1080 * 5 - 1392);
                fire2[10].setPosition(1630, background_out.getPosition().y + 1080 * 5 - 1392);
                base_wood.setPosition(0, background.getPosition().y + 395);
                collision_shapes[3].setPosition(0, background.getPosition().y + 426);
                window.draw(fire[10]);
                window.draw(fire2[10]);
                window.draw(base_wood);
                for (int i = 0; i < 10; i++) {
                    fire[i].setPosition(260, background_out.getPosition().y + Fpos - 1220);
                    fire2[i].setPosition(1660, background_out.getPosition().y + Fpos - 1220);
                    Fpos += 1080;
                    window.draw(fire[i]);
                    window.draw(fire2[i]);
                    if (i == 9) Fpos = 1080 * 6;
                }
                window.draw(background_out);
                window.draw(bro);
                window.draw(health[0]);
                window.draw(health[1]);
                window.draw(pyramid);
                for (int i = 0; i < 3; i++) window.draw(heart[i]);
                window.draw(health[2]);
                window.draw(Time);
                window.draw(TIME);
                window.draw(slot);
                if (!replay_lvl3) {
                    window.draw(shape99);
                    window.draw(syntax);
                }
                window.draw(rect);
                //window.draw(HitBox);
                window.display();
            }
            deltaTime = c.getElapsedTime().asSeconds();
        }
    }
}

void gameWindow5(RenderWindow& window) {
    edit_data();
    RectangleShape black_screen(Vector2f(1920, 1080));
    black_screen.setFillColor(Color(0, 0, 0, 190));

    int shooting_max_delay;
    if (!hard) shooting_max_delay = 10;
    else shooting_max_delay = 20;

    ConvexShape rect(5);
    rect.setFillColor(Color(120, 120, 120, 200));
    rect.setPoint(0, Vector2f(526, 675));
    rect.setPoint(1, Vector2f(1407, 675));
    rect.setPoint(2, Vector2f(1407, 944));
    rect.setPoint(3, Vector2f(955, 1065));
    rect.setPoint(4, Vector2f(526, 944));
    ConvexShape rect2(5);
    rect2.setFillColor(Color(120, 120, 120, 200));
    rect2.setPoint(0, Vector2f(127, 323));
    rect2.setPoint(1, Vector2f(323, 323));
    rect2.setPoint(2, Vector2f(323, 617));
    rect2.setPoint(3, Vector2f(157, 766));
    rect2.setPoint(4, Vector2f(127, 766));
    ConvexShape rect3(5);
    rect3.setFillColor(Color(120, 120, 120, 200));
    rect3.setPoint(0, Vector2f(1607, 435));
    rect3.setPoint(1, Vector2f(1820, 435));
    rect3.setPoint(2, Vector2f(1820, 684));
    rect3.setPoint(3, Vector2f(1778, 684));
    rect3.setPoint(4, Vector2f(1607, 529));
    RectangleShape rect4(Vector2f(200, 500));
    rect4.setFillColor(Color(120, 120, 120, 200));
    rect4.setPosition(306, 300);
    RectangleShape rect5(Vector2f(200, 500));
    rect5.setFillColor(Color(120, 120, 120, 200));
    rect5.setPosition(1427, 300);
    RectangleShape rect6(Vector2f(213, 50));
    rect6.setFillColor(Color(120, 120, 120, 200));
    rect6.setPosition(1607, 400);
    RectangleShape rect7(Vector2f(196, 50));
    rect7.setFillColor(Color(120, 120, 120, 200));
    rect7.setPosition(127, 300);

    RectangleShape boss_hitBox(Vector2f(90, 100));
    boss_hitBox.setFillColor(Color::Transparent);
    boss_hitBox.setOutlineThickness(2);
    boss_hitBox.setOutlineColor(Color::Red);
    boss_hitBox.setPosition(10000, 250);

    Texture VICTORY_T;
    VICTORY_T.loadFromFile("./assets/textures/VICTORY.png");
    Sprite VICTORY;
    VICTORY.setTexture(VICTORY_T);
    Color spriteColor = VICTORY.getColor();

    //bg
    Texture bG_T;
    bG_T.loadFromFile("./assets/textures/level 5 ground.png");
    Texture bG_T2;
    bG_T2.loadFromFile("./assets/textures/level 5 ground2.png");
    Texture bg_T;
    bg_T.loadFromFile("./assets/textures/level 5 background.png");
    Sprite ground;
    ground.setTexture(bG_T);
    Sprite ground2;
    ground2.setTexture(bG_T2);
    Sprite background;
    background.setTexture(bg_T);

    //ability
    //1 fire ball
    //2 shield
    //3 health

    Texture ability_t[4];
    ability_t[0].loadFromFile("./assets/textures/ability1.png");
    ability_t[1].loadFromFile("./assets/textures/ability2.png");
    ability_t[2].loadFromFile("./assets/textures/ability3.png");
    ability_t[3].loadFromFile("./assets/textures/ability4.png");
    Sprite ability[4];
    for (int i = 0; i < 4; i++) {
        ability[i].setTexture(ability_t[i]);
        ability[i].setScale(0.1 * 0.86051041667, 0.1 * 0.86051041667);
        ability[i].setPosition(17000, 0);
    }

    //player
    Texture player_t;
    player_t.loadFromFile("./assets/textures/Hero_P1.png");
    Texture player_d;
    player_d.loadFromFile("./assets/textures/Hero_P2.png");
    Texture player_dd;
    player_dd.loadFromFile("./assets/textures/Sword_sprite2.png");
    Sprite bro;
    bro.setTexture(player_t);
    bro.setPosition(1000, 250);
    Vector2f pos_right_D;
    Vector2f pos_left_A;

    //shield
    Texture shield_T;
    shield_T.loadFromFile("./assets/textures/shield.png");
    Sprite shield;
    shield.setTexture(shield_T);
    shield.setScale(0.4, 0.4);
    shield.setOrigin(shield.getTextureRect().width / 2, shield.getTextureRect().height / 2);

    //shoothing
    Texture ammoT[2];
    ammoT[0].loadFromFile("./assets/textures/shoot.png");
    ammoT[1].loadFromFile("./assets/textures/shoot2.png");
    Sprite ammo[2];
    for (int i = 0; i < 2; i++) {
        ammo[i].setTexture(ammoT[0]);
        ammo[i].setTextureRect(IntRect(0, 0, 250, 174));
    }
    ammo[0].setScale(0.1, 0.1);
    ammo[1].setScale(-0.1, 0.1);
    vector<Sprite> ammos;
    ammos.push_back(Sprite(ammo[0]));
    vector<float> angles;
    angles.push_back(atan2(Mouse::getPosition(window).y - bro.getPosition().y, Mouse::getPosition(window).x - bro.getPosition().x));

    //boss
    Texture boss_T;
    boss_T.loadFromFile("./assets/textures/level 5 boss.png");
    Sprite boss;
    boss.setTexture(boss_T);
    boss.setPosition(10000, 250);
    boss.setTextureRect(IntRect(0, 0, 0, 0));
    boss.setScale(2, 2);
    Sprite boss_magic;
    boss_magic.setTexture(boss_T);
    boss_magic.setPosition(10000, 250);
    boss_magic.setTextureRect(IntRect(0, 0, 0, 0));
    boss_magic.setScale(2.2, 2.2);

    //player health
    Texture health_T[3];
    health_T[0].loadFromFile("./assets/textures/health.png");
    health_T[1].loadFromFile("./assets/textures/health2.png");
    health_T[2].loadFromFile("./assets/textures/health3.png");
    Sprite health[3];
    health[1].setPosition(141 * 0.86051041667, 116 * 0.86051041667);
    for (int i = 0; i < 3; i++) {
        health[i].setTexture(health_T[i]);
        health[i].setPosition(health[i].getPosition().x + 38 + 100, health[i].getPosition().y + 10 + 68);
        health[i].setScale(0.266 * 0.86051041667, 0.266 * 0.86051041667);
    }
    Texture heart_T;
    heart_T.loadFromFile("./assets/textures/heart.png");
    Sprite heart[3];
    for (int i = 0; i < 3; i++) {
        heart[i].setTexture(heart_T);
        heart[i].setTextureRect(IntRect(0, 0, 125.1, 126));
        heart[i].setScale(0.3 * 0.86051041667, 0.3 * 0.86051041667);
    }
    heart[2].setPosition(148.46, 125.5 * 0.86051041667 + 68);
    heart[1].setPosition(148.46 + (126 * 0.3 * 0.86051041667), 125.5 * 0.86051041667 + 68);
    heart[0].setPosition(148.46 + (252 * 0.3 * 0.86051041667), 125.5 * 0.86051041667 + 68);

    //boss health bar
    Texture B_Hbar[3];
    B_Hbar[0].loadFromFile("./assets/textures/boss_Hbar1.png");
    B_Hbar[1].loadFromFile("./assets/textures/boss_Hbar2.png");
    B_Hbar[2].loadFromFile("./assets/textures/boss_Hbar3.png");
    Sprite boss_Hbar[3];
    for (int i = 0; i < 3; i++) {
        boss_Hbar[i].setTexture(B_Hbar[i]);
        boss_Hbar[i].setScale(0.266 * 0.86051041667, 0.266 * 0.86051041667);
        boss_Hbar[i].setPosition(133, 85);
    }
    boss_Hbar[0].setPosition(boss_Hbar[1].getPosition().x + (356 * 0.266 * 0.86051041667), 3734 * 0.266 * 0.86051041667 + 85);
    boss_Hbar[1].setPosition(boss_Hbar[1].getPosition().x + (356 * 0.266 * 0.86051041667), 3734 * 0.266 * 0.86051041667 + 85);


    //slots
    Texture slot_T[5];
    slot_T[0].loadFromFile("./assets/textures/slot 0.png");
    slot_T[1].loadFromFile("./assets/textures/slot 001.png");
    slot_T[2].loadFromFile("./assets/textures/slot 002.png");
    slot_T[3].loadFromFile("./assets/textures/slot 003.png");
    slot_T[4].loadFromFile("./assets/textures/slot 004.png");
    Sprite slot;
    slot.setTexture(slot_T[0]);
    slot.setPosition(slot.getPosition().x + 138, slot.getPosition().y + 10 + 68);
    slot.setScale(0.266 * 0.86051041667, 0.266 * 0.86051041667);
    Texture SSlot_T[4];
    SSlot_T[0].loadFromFile("./assets/textures/ability1_00.png");
    SSlot_T[1].loadFromFile("./assets/textures/ability2_00.png");
    SSlot_T[2].loadFromFile("./assets/textures/ability3_00.png");
    SSlot_T[3].loadFromFile("./assets/textures/ability4_00.png");
    Sprite Slots[4];
    for (int i = 0; i < 4; i++) {
        Slots[i].setTexture(SSlot_T[i]);
        Slots[i].setPosition(-5000, -5000);
        Slots[i].setScale(0.266 * 0.86051041667, 0.266 * 0.86051041667);
    }

    //timer
    Texture timer_T[2];
    timer_T[0].loadFromFile("./assets/textures/timer.png");
    timer_T[1].loadFromFile("./assets/textures/timer_end.png");
    Sprite Time;
    Time.setTexture(timer_T[0]);
    Time.setScale(0.266 * 0.86051041667, 0.266 * 0.86051041667);
    Time.setTextureRect(IntRect(0, 0, 490, 348));
    Time.setPosition((1920 / 2) - (Time.getGlobalBounds().width / 2), Time.getPosition().y + 10 + 68);

    //timer Text
    Font font;
    font.loadFromFile("./assets/font/trebuc.ttf");
    Text TIME;
    TIME.setFont(font);
    TIME.setCharacterSize(50 * 0.86051041667);
    TIME.setFillColor(Color::Black);
    TIME.setPosition((1920 / 2) - (Time.getGlobalBounds().width / 2) + 16, Time.getPosition().y + 7);

    //pyramid
    Texture pyramid_T;
    pyramid_T.loadFromFile("./assets/textures/pyramid.png");
    Sprite pyramid;
    pyramid.setTexture(pyramid_T);
    pyramid.setPosition(148.5, 20 + 68);
    pyramid.setScale(0.22 * 0.86051041667, 0.3 * 0.86051041667);

    //fire balls
    Texture balls_T;
    balls_T.loadFromFile("./assets/textures/fire_balls.png");
    Sprite fireballs[8];
    int H_deff = 0;
    for (int i = 0; i < 8; i++) {
        fireballs[i].setTexture(balls_T);
        fireballs[i].setScale(0.25, 0.25);
        fireballs[i].setPosition(rand() % 1652, -400 - H_deff); //443 + rand() % 937
        H_deff += 170;
        fireballs[i].rotate(-45);
    }
    int speed_fireballs = 800;

    Music foot[2];
    foot[0].openFromFile("./assets/sounds/foot.mp3");
    foot[0].setVolume(44);
    foot[0].setLoop(true);
    foot[1].openFromFile("./assets/sounds/foot 2x.mp3");
    foot[1].setVolume(44);
    foot[1].setLoop(true);
    Music jump;
    jump.openFromFile("./assets/sounds/jump.mp3");
    jump.setVolume(69);
    Music hurt;
    hurt.openFromFile("./assets/sounds/hurt.mp3");
    hurt.setVolume(100);
    Music you_are_dead[2];
    you_are_dead[0].openFromFile("./assets/sounds/YOU ARE DEAD 2.wav");
    you_are_dead[1].openFromFile("./assets/sounds/you-re DEAD 2.wav");
    Music ability_s;
    ability_s.openFromFile("./assets/sounds/coin collected.wav");
    ability_s.setVolume(89);
    Music knife;
    knife.openFromFile("./assets/sounds/knife.mp3");
    knife.setVolume(30);
    Music shoot;
    if (!hard) shoot.openFromFile("./assets/sounds/shoot 3x.mp3");
    else shoot.openFromFile("./assets/sounds/shoot 2x.mp3");
    shoot.setVolume(60);
    shoot.setLoop(true);
    Music punch;
    punch.openFromFile("./assets/sounds/punch 2x.mp3");
    punch.setVolume(10);
    Music gate_sound;
    gate_sound.openFromFile("./assets/sounds/gate.mp3");
    Music game_over_sound;
    game_over_sound.openFromFile("./assets/sounds/game_over_sound.mp3");
    Music YOU_WIN;
    YOU_WIN.openFromFile("./assets/sounds/YOU WIN 5.wav");
    Music music_bg;
    music_bg.openFromFile("./assets/sounds/bg_music_FULL.mp3");
    music_bg.setVolume(28 * volume);
    music_bg.setLoop(true);
    Music end_of_game;
    end_of_game.openFromFile("./assets/sounds/end_of_game (1).mp3");
    end_of_game.setVolume(39);
    Music nyzak;
    nyzak.openFromFile("./assets/sounds/nyzak.mp3");
    nyzak.setVolume(15);

    //syntax
    String text;
    Text syntax;
    syntax.setFont(font);
    syntax.setCharacterSize(25 * 0.86051041667);
    RectangleShape shape99;
    shape99.setFillColor(Color(0, 0, 0, 140));
    Music lvl4;
    lvl4.openFromFile("./assets/sounds/easy-hard-mode-lvl4.mp3");
    if (!replay_lvl4) lvl4.play();

    //shake efx
    const float shakeTime = 10;
    const float shakeIntensity = 9; // intensity of shake
    Clock shakeClock;

    View view(FloatRect(0, 0, 1920, 1080));
    Clock c;
    Vector2f pos_jump(0, 0);
    float COUNTER = 0, E_COUNTER = 0, deltaTime = 0, timer_count = 0, timer0 = 0, delay_attack = 0.05, timer3 = 0, timer_boss = 0, timer_magic_boss = 0, timer = 0, zoomfloat = 0, gravity = 20, jumphight = 29, timer4 = 0, timer5 = 0;
    float timer6 = 0, timer7 = 0, timer9 = 0, timer11 = 0, C_COUNTER = 0, COUNTER_ABILITY2 = 0, COUNTER_ABILITY4 = 0;
    int speed = 160, boss_speed = 150, ii = 0, vii = 0, iii = 0, iiii = 0, i_boss = 9, i_magic_boss = 0, v = 0, joined = 0, TIME_Display = 100, t = 0, p = 0, fireballs_iterator = 0;
    int heart_anime = 0, heart_num = 0, ability_number = 0, ability_collect = 0, shoot_delay = 0, shoot_delay2 = 0, shoot_speed = 550, shoot_animation = 0, iv = 0, delay = .99, abilities_collecte = 0;
    int pause_count = 0, opacityChange = 1;
    bool increasingOpacity = true;
    bool grounded = true;
    bool dead = false;
    bool boss_AFK = false, near = false;
    bool traps = false;
    char status = 'D';
    bool ability_2 = false, ability_3 = false, ability_4 = false, ability_1 = false;
    Vector2f playerCenter;

    if (sound) music_bg.play();
    while (window.isOpen()) {
        bool play = true;
        c.restart();
        if (PageNumber != 5) {
            window.setView(window.getDefaultView());
            break;
        }
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    while (pause_count < 2) {
                        pause_count++;
                        window.draw(black_screen);
                        window.display();
                    }
                    pause_count = 0;
                    music_bg.pause();
                    if (foot[0].getStatus() == Music::Playing) foot[0].pause();
                    if (foot[1].getStatus() == Music::Playing) foot[1].pause();
                    if (shoot.getStatus() == Music::Playing) shoot.pause();
                    if (ability_s.getStatus() == Music::Playing) ability_s.pause();
                    if (jump.getStatus() == Music::Playing) jump.pause();
                    if (knife.getStatus() == Music::Playing) knife.pause();
                    if (punch.getStatus() == Music::Playing) punch.pause();
                    if (hurt.getStatus() == Music::Playing) hurt.pause();
                    if (you_are_dead[0].getStatus() == Music::Playing) you_are_dead[0].pause();
                    if (you_are_dead[1].getStatus() == Music::Playing) you_are_dead[1].pause();
                    if (gate_sound.getStatus() == Music::Playing) gate_sound.pause();
                    if (lvl4.getStatus() == Music::Playing) lvl4.pause();
                    if (end_of_game.getStatus() == Music::Playing) end_of_game.pause();
                    if (nyzak.getStatus() == Music::Playing) nyzak.pause();
                    window.setView(window.getDefaultView());
                    pause(window);
                    window.setView(view);
                    if (PageNumber != 5) {
                        window.setView(window.getDefaultView());
                        return;
                    }
                    if (sound) music_bg.play();
                    if (foot[0].getStatus() == Music::Paused) foot[0].play();
                    if (foot[1].getStatus() == Music::Paused) foot[1].play();
                    if (shoot.getStatus() == Music::Paused) shoot.play();
                    if (ability_s.getStatus() == Music::Paused) ability_s.play();
                    if (jump.getStatus() == Music::Paused) jump.play();
                    if (knife.getStatus() == Music::Paused) knife.play();
                    if (punch.getStatus() == Music::Paused) punch.play();
                    if (hurt.getStatus() == Music::Paused) hurt.play();
                    if (you_are_dead[0].getStatus() == Music::Paused) you_are_dead[0].play();
                    if (you_are_dead[1].getStatus() == Music::Paused) you_are_dead[1].play();
                    if (gate_sound.getStatus() == Music::Paused) gate_sound.play();
                    if (lvl4.getStatus() == Music::Paused) lvl4.play();
                    if (end_of_game.getStatus() == Music::Paused) end_of_game.play();
                    if (nyzak.getStatus() == Music::Paused) nyzak.play();

                    play = false;
                    break;
                }
            }
        }
        if (play) {
            bool intersects = false;
            bool magic_intersects = false;
            bool ball_intersect = false;
            bool shooting = Mouse::isButtonPressed(Mouse::Left) ? true : false;

            if (boss_Hbar[1].getScale().x <= 0.000892) {    //WIN
                music_bg.stop();
                foot[0].stop();
                foot[1].stop();
                shoot.stop();
                ability_s.stop();
                jump.stop();
                knife.stop();
                punch.stop();
                hurt.stop();
                you_are_dead[0].stop();
                you_are_dead[1].stop();
                gate_sound.stop();
                lvl4.stop();
                end_of_game.stop();
                nyzak.stop();
                YOU_WIN.play();
                i_boss = 0;
                COUNTER = 0;
                timer_boss = 0;
                float OBS = 0;
                bro.setTexture(player_t);
                while (COUNTER < 10) {
                    while (window.pollEvent(event)) {
                        if (event.type == Event::Closed) {
                            window.close();
                        }
                    }
                    if (increasingOpacity) {
                        spriteColor.a += opacityChange;
                        if (spriteColor.a >= 255) {
                            spriteColor.a = 255;
                            increasingOpacity = false;
                        }
                    }
                    else {
                        spriteColor.a -= opacityChange;
                        if (spriteColor.a <= 0) {
                            spriteColor.a = 0;
                            increasingOpacity = true;
                        }
                    }
                    VICTORY.setColor(spriteColor);
                    sleep(sf::milliseconds(10));
                    if (i_boss < 11) {
                        boss.setTextureRect(IntRect(i_boss * 140, 372, 140, 93));
                        i_boss++;
                    }

                    if (status == 'D') {
                        if (timer < 0) {
                            bro.setTextureRect(IntRect(iii * 19, 35, 19, 35));
                            iii++;
                            iii %= 8;
                            timer = 0.0055;
                        }
                        else {
                            timer -= deltaTime;
                        }
                    }
                    else if (status == 'A') {
                        if (timer < 0) {
                            bro.setTextureRect(IntRect(iii * 19, 105, 19, 35));
                            iii++;
                            iii %= 8;
                            timer = 0.0055;
                        }
                        else {
                            timer -= deltaTime;
                        }
                    }
                    if (OBS < 255)
                        OBS += 0.5;
                    black_screen.setFillColor(Color(0, 0, 0, OBS));
                    COUNTER += 0.01;
                    window.setView(view);
                    window.clear();
                    window.draw(background);
                    window.draw(ground);
                    window.draw(bro);
                    window.draw(boss);
                    window.draw(ground2);
                    window.setView(window.getDefaultView());
                    window.draw(black_screen);
                    window.draw(VICTORY);
                    window.display();
                }
                if (COUNTER >= 10) {
                    PageNumber = -3;
                    window.setView(window.getDefaultView());
                    loading(window, 1000);
                    return;
                }
            }


            //player animation
            bool AFK = true;
            bro.setScale(3 * 0.860510419, 3 * 0.860510419);
            if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::LShift) && grounded && bro.getGlobalBounds().intersects(rect.getGlobalBounds())) {
                if (foot[1].getStatus() == Music::Stopped) {
                    foot[1].play();
                }
                foot[0].stop();
                shoot.stop();
                pos_left_A.x = 0;
                if (timer < 0) {
                    bro.setTextureRect(IntRect(vii * 29, 283, 29, 30));
                    vii++;
                    vii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'D';
                AFK = false;
                pos_right_D.x = speed * 2 * deltaTime;
            }
            else if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::LShift) && grounded && bro.getGlobalBounds().intersects(rect.getGlobalBounds())) {
                if (foot[1].getStatus() == Music::Stopped) {
                    foot[1].play();
                }
                shoot.stop();
                foot[0].stop();
                pos_right_D.x = 0;
                if (timer < 0) {
                    bro.setTextureRect(IntRect(vii * 29, 313, 29, 30));
                    vii++;
                    vii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'A';
                AFK = false;
                pos_left_A.x = -speed * 2 * deltaTime;
            }

            else if (Keyboard::isKeyPressed(Keyboard::D) && (!shooting)) {
                //moving sound
                shoot.stop();
                if (grounded) {
                    if (foot[0].getStatus() == Music::Stopped) foot[0].play();
                    foot[1].stop();
                }
                else {
                    foot[0].stop();
                    foot[1].stop();
                }

                pos_left_A.x = 0;
                if (timer < 0) {
                    bro.setTextureRect(IntRect(ii * 20, 175, 20, 35));
                    ii++;
                    ii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'D';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    pos_right_D.x = speed * deltaTime;
                }
                else {
                    pos_right_D.x = speed * deltaTime;
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::A) && (!shooting)) {
                pos_right_D.x = 0;
                //moving sound
                shoot.stop();
                if (grounded) {
                    if (foot[0].getStatus() == Music::Stopped) foot[0].play();
                    foot[1].stop();
                }
                else {
                    foot[0].stop();
                    foot[1].stop();
                }

                pos_right_D.x = 0;

                if (timer < 0) {
                    bro.setTextureRect(IntRect(ii * 20, 248, 20, 35));
                    ii++;
                    ii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
                status = 'A';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    pos_left_A.x = -speed * deltaTime;
                }
                else {
                    pos_left_A.x = -speed * deltaTime;
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::D) && shooting) {
                if (shoot.getStatus() == Music::Stopped) shoot.play();
                //moving sound
                if (foot[0].getStatus() == Music::Playing)  foot[0].play();
                else if (foot[1].getStatus() == Music::Playing)  foot[1].stop();

                pos_left_A.x = 0;
                if (timer0 < 0) {
                    bro.setTextureRect(IntRect(iv * 22, 343, 22, 32));
                    iv++;
                    iv %= 6;
                    timer0 = delay_attack;
                }
                else {
                    timer0 -= deltaTime;
                }
                status = 'D';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    background.move(speed * deltaTime, 0);

                    for (int i = 0; i < ammos.size(); i++) {
                        ammos[i].move(speed * deltaTime, 0);
                    }

                }
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    pos_right_D.x = speed * deltaTime;
                }
                else {
                    pos_right_D.x = speed * deltaTime;
                }
                //shooting D
                if (shoot_delay > shooting_max_delay) {
                    shoot_delay = 0;
                    ammo[0].setPosition(playerCenter);
                    ammos.push_back(Sprite(ammo[0]));
                    angles.push_back(atan2(Mouse::getPosition(window).y - 51 - bro.getPosition().y, Mouse::getPosition(window).x - bro.getPosition().x));
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::A) && shooting) {
                if (shoot.getStatus() == Music::Stopped) shoot.play();
                //moving sound
                if (foot[0].getStatus() == Music::Playing)  foot[0].play();
                else if (foot[1].getStatus() == Music::Playing)  foot[1].stop();

                pos_right_D.x = 0;
                if (timer0 < 0) {
                    bro.setTextureRect(IntRect(iv * 22, 375, 22, 32));
                    iv++;
                    iv %= 6;
                    timer0 = delay_attack;
                }
                else {
                    timer0 -= deltaTime;
                }
                status = 'A';
                AFK = false;
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    background.move(speed * deltaTime, 0);
                    for (int i = 0; i < ammos.size(); i++) {
                        ammos[i].move(speed * deltaTime, 0);
                    }
                }
                if (bro.getPosition().x > 300 && background.getPosition().x <= 0 && background.getPosition().x > -11200) {
                    pos_left_A.x = -speed * deltaTime;
                }
                else {
                    pos_left_A.x = -speed * deltaTime;
                }
                //shooting A
                if (shoot_delay > shooting_max_delay) {
                    shoot_delay = 0;
                    ammo[1].setPosition(playerCenter);
                    ammos.push_back(Sprite(ammo[1]));
                    angles.push_back(atan2(Mouse::getPosition(window).y - 51 - bro.getPosition().y, Mouse::getPosition(window).x - bro.getPosition().x));
                }
            }
            else if (status == 'D' && shooting) {
                //moving sound
                if (shoot.getStatus() == Music::Stopped) shoot.play();
                if (foot[0].getStatus() == Music::Playing)  foot[0].stop();
                else if (foot[1].getStatus() == Music::Playing)  foot[1].stop();

                pos_left_A.x = 0;
                pos_right_D.x = 0;
                if (timer0 < 0) {
                    bro.setTextureRect(IntRect(iv * 22, 407, 22, 33));
                    iv++;
                    iv %= 6;
                    timer0 = delay_attack;
                }
                else {
                    timer0 -= deltaTime;
                }
                status = 'D';
                AFK = false;
                //shooting D (not moving)
                if (shoot_delay > shooting_max_delay) {
                    shoot_delay = 0;
                    ammo[0].setPosition(playerCenter);
                    ammos.push_back(Sprite(ammo[0]));
                    angles.push_back(atan2(Mouse::getPosition(window).y - 51 - bro.getPosition().y, Mouse::getPosition(window).x - bro.getPosition().x));
                }
            }
            else if (status == 'A' && shooting) {
                //moving sound
                if (shoot.getStatus() == Music::Stopped) shoot.play();
                if (foot[0].getStatus() == Music::Playing)  foot[0].stop();
                else if (foot[1].getStatus() == Music::Playing)  foot[1].stop();

                foot[1].stop();
                pos_left_A.x = 0;
                pos_right_D.x = 0;
                if (timer0 < 0) {
                    bro.setTextureRect(IntRect(iv * 22, 440, 22, 33));
                    iv++;
                    iv %= 6;
                    timer0 = delay_attack;
                }
                else {
                    timer0 -= deltaTime;
                }
                status = 'A';
                AFK = false;
                //shooting A (not moving)
                if (shoot_delay > shooting_max_delay) {
                    shoot_delay = 0;
                    ammo[1].setPosition(playerCenter);
                    ammos.push_back(Sprite(ammo[1]));
                    angles.push_back(atan2(Mouse::getPosition(window).y - 51 - bro.getPosition().y, Mouse::getPosition(window).x - bro.getPosition().x));
                }
            }
            else {
                shoot.stop();
                foot[0].stop();
                foot[1].stop();
                pos_right_D.x = 0;
                pos_left_A.x = 0;
            }

            if (Mouse::isButtonPressed(Mouse::Left) && (Mouse::getPosition(window).x - bro.getPosition().x > 0))
                status = 'D';
            else if (Mouse::isButtonPressed(Mouse::Left) && bro.getPosition().x - Mouse::getPosition(window).x > 0)
                status = 'A';

            float sizes = 0.7;
            ground.move(-pos_right_D.x * sizes, -pos_right_D.y * sizes);
            ground.move(-pos_left_A.x * sizes, -pos_left_A.y * sizes);
            ground2.move(-pos_right_D.x * sizes, -pos_right_D.y * sizes);
            ground2.move(-pos_left_A.x * sizes, -pos_left_A.y * sizes);
            rect.move(-pos_right_D.x * sizes, -pos_right_D.y * sizes);
            rect.move(-pos_left_A.x * sizes, -pos_left_A.y * sizes);
            rect2.move(-pos_right_D.x * sizes, -pos_right_D.y * sizes);
            rect2.move(-pos_left_A.x * sizes, -pos_left_A.y * sizes);
            rect3.move(-pos_right_D.x * sizes, -pos_right_D.y * sizes);
            rect3.move(-pos_left_A.x * sizes, -pos_left_A.y * sizes);
            rect4.move(-pos_right_D.x * sizes, -pos_right_D.y * sizes);
            rect4.move(-pos_left_A.x * sizes, -pos_left_A.y * sizes);
            rect5.move(-pos_right_D.x * sizes, -pos_right_D.y * sizes);
            rect5.move(-pos_left_A.x * sizes, -pos_left_A.y * sizes);
            rect6.move(-pos_right_D.x * sizes, -pos_right_D.y * sizes);
            rect6.move(-pos_left_A.x * sizes, -pos_left_A.y * sizes);
            rect7.move(-pos_right_D.x * sizes, -pos_right_D.y * sizes);
            rect7.move(-pos_left_A.x * sizes, -pos_left_A.y * sizes);
            for (int i = 0; i < 4; i++) {
                ability[i].move(-pos_right_D.x * sizes, -pos_right_D.y * sizes);
                ability[i].move(-pos_left_A.x * sizes, -pos_left_A.y * sizes);
            }
            for (int i = 0; i < 8; i++) {
                fireballs[i].move(-pos_left_A.x * sizes, -pos_left_A.y * sizes);
                fireballs[i].move(-pos_right_D.x * sizes, -pos_right_D.y * sizes);
            }
            //coins[3].move(-pos_right_D.x * sizes, -pos_right_D.y * sizes);
            //coins[3].move(-pos_left_A.x * sizes, -pos_left_A.y * sizes);
            bro.move(pos_right_D);
            bro.move(pos_left_A);

            //player AFK movement
            if (status == 'D' && AFK) {

                if (timer < 0) {
                    bro.setTextureRect(IntRect(iii * 19, 35, 19, 35));
                    iii++;
                    iii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
            }
            else if (status == 'A' && AFK) {
                if (timer < 0) {
                    bro.setTextureRect(IntRect(iii * 19, 105, 19, 35));
                    iii++;
                    iii %= 8;
                    timer = 0.08;
                }
                else {
                    timer -= deltaTime;
                }
            }

            //zoom
            if (zoomfloat < 50) {
                if (timer3 < 0) {
                    view.zoom(0.997);
                    zoomfloat++;
                    timer3 = 0.05;
                }
                else {
                    timer3 -= deltaTime;
                }
            }

            //shooting
            shoot_delay++;
            if (gravity < 0) {
                playerCenter = Vector2f(bro.getPosition().x + (bro.getGlobalBounds().width / 2), (bro.getPosition().y + (bro.getGlobalBounds().height / 2)) - 100);
            }
            else {
                playerCenter = Vector2f(bro.getPosition().x + (bro.getGlobalBounds().width / 2), bro.getPosition().y + (bro.getGlobalBounds().height / 2));
            }
            for (int i = 0; i < ammos.size(); i++) {
                if (timer9 < 0) {
                    ammos[i].setTextureRect(IntRect(shoot_animation * 250, 0, 250, 174));
                    if (i == ammos.size() - 1) {
                        shoot_animation++;
                        shoot_animation %= 4;
                        timer9 = 0.15;
                    }
                }
                else {
                    i = ammos.size() - 1;
                    timer9 -= deltaTime;
                }
            }
            for (int i = 0; i < ammos.size(); i++) {
                ammos[i].move(shoot_speed * cos(angles[i]) * deltaTime, shoot_speed * sin(angles[i]) * deltaTime);

                if (abs(ammos[i].getPosition().x - bro.getPosition().x) > 600 || abs(ammos[i].getPosition().y - bro.getPosition().y) > 400) {
                    ammos.erase(ammos.begin() + i);
                    angles.erase(angles.begin() + i);
                }
            }

            //collesion
            FloatRect broBounds = bro.getGlobalBounds();
            FloatRect rectBounds = rect.getGlobalBounds();
            FloatRect rect2Bounds = rect2.getGlobalBounds();
            FloatRect rect3Bounds = rect3.getGlobalBounds();
            FloatRect rect4Bounds = rect4.getGlobalBounds();
            FloatRect rect5Bounds = rect5.getGlobalBounds();
            FloatRect rect6Bounds = rect6.getGlobalBounds();
            FloatRect rect7Bounds = rect7.getGlobalBounds();
            FloatRect bossBounds = boss.getGlobalBounds();

            //rect
            if (broBounds.intersects(rectBounds)) {
                Vector2f correctionVector;

                float xOverlap = min(broBounds.left + broBounds.width - rectBounds.left, rectBounds.left + rectBounds.width - broBounds.left);
                float yOverlap = min(broBounds.top + broBounds.height - rectBounds.top, rectBounds.top + rectBounds.height - broBounds.top);

                if (xOverlap < yOverlap) {

                    correctionVector.x = (broBounds.left < rectBounds.left) ? -xOverlap : xOverlap;
                    bro.move(0, 300 * deltaTime);
                }
                else {
                    grounded = true;
                    correctionVector.y = (broBounds.top < rectBounds.top) ? -yOverlap : yOverlap;
                }
                bro.move(correctionVector);

                speed = 190;
                pos_jump.y = 0;

            }
            else
            {
                speed = 330;
                pos_jump.y += gravity * deltaTime;
            }

            //rect2
            if (broBounds.intersects(rect2Bounds)) {
                Vector2f correctionVector;
                float xOverlap = min(broBounds.left + broBounds.width - rect2Bounds.left, rect2Bounds.left + rect2Bounds.width - broBounds.left);
                float yOverlap = min(broBounds.top + broBounds.height - rect2Bounds.top, rect2Bounds.top + rect2Bounds.height - broBounds.top);

                if (xOverlap < yOverlap) {

                    correctionVector.x = (broBounds.left < rect2Bounds.left) ? -xOverlap : xOverlap;
                    bro.move(0, 300 * deltaTime);
                }
                else {
                    correctionVector.y = (broBounds.top < rect2Bounds.top) ? -yOverlap : yOverlap;
                    grounded = true;
                }
                bro.move(correctionVector);

                speed = 190;
                pos_jump.y = 0;

            }
            else
            {
                speed = 330;
                pos_jump.y += gravity * deltaTime;
            }

            //rect3
            if (broBounds.intersects(rect3Bounds)) {
                Vector2f correctionVector;
                float xOverlap = min(broBounds.left + broBounds.width - rect3Bounds.left, rect3Bounds.left + rect3Bounds.width - broBounds.left);
                float yOverlap = min(broBounds.top + broBounds.height - rect3Bounds.top, rect3Bounds.top + rect3Bounds.height - broBounds.top);

                if (xOverlap < yOverlap) {

                    correctionVector.x = (broBounds.left < rect3Bounds.left) ? -xOverlap : xOverlap;
                    bro.move(0, 300 * deltaTime);
                }
                else {
                    correctionVector.y = (broBounds.top < rect3Bounds.top) ? -yOverlap : yOverlap;
                    grounded = true;
                }
                bro.move(correctionVector);

                speed = 190;
                pos_jump.y = 0;

            }
            else
            {
                speed = 320;
                pos_jump.y += gravity * deltaTime;
            }
            if (Keyboard::isKeyPressed(Keyboard::Space) && grounded)
            {
                jump.play();
                pos_jump.y -= jumphight;
                grounded = false;
            }
            bro.move(pos_jump);

            //respawn player
            if (bro.getPosition().y > 1200 && COUNTER > 2) {
                health[1].setScale(0, 0);
                heart_num = 3;
            }
            else if (bro.getPosition().y > 1200)
                bro.setPosition(925, 100);

            //boss animation
            boss_hitBox.setPosition(boss.getPosition());
            if (boss.getScale().x < 1)
                boss_hitBox.setOrigin((boss.getTextureRect().width / 1.28) - 64, -80);
            else
                boss_hitBox.setOrigin((boss.getTextureRect().width / 1.28) - 60, -80);
            boss.setOrigin(boss.getTextureRect().width / 1.28, 0);
            int animation1_time = 18;
            if (COUNTER < animation1_time) {
                if (E_COUNTER > 2 && E_COUNTER < 2.7) {
                    if (boss.getScale().x > 1) boss.setPosition(ground.getPosition().x + 1722, 250);
                    else boss.setPosition(ground.getPosition().x + 227, 138);

                    if (timer_boss < 0) {
                        boss.setTextureRect(IntRect(i_boss * 140, 372, 140, 93));
                        i_boss--;
                        timer_boss = 0.09;
                    }
                    else {
                        timer_boss -= deltaTime;
                    }
                }
                else if (E_COUNTER >= 2.7 && !near) {
                    if (boss.getScale().x > 1) boss.setPosition(ground.getPosition().x + 1722, 250);
                    else boss.setPosition(ground.getPosition().x + 227, 138);

                    if (timer_boss < 0) {
                        boss.setTextureRect(IntRect(i_boss * 140, 0, 140, 93));
                        i_boss++;
                        i_boss %= 8;
                        timer_boss = 0.09;
                    }
                    else {
                        timer_boss -= deltaTime;
                    }
                }
                if (COUNTER < float(animation1_time - 1.1)) {
                    if (boss.getPosition().x > bro.getPosition().x && !near && E_COUNTER >= 2.9) {
                        if (boss.getPosition().x - bro.getPosition().x < 131) {
                            near = true;
                        }
                    }
                    else if (boss.getPosition().x < bro.getPosition().x && !near && E_COUNTER >= 2.9) {
                        if (bro.getPosition().x - boss.getPosition().x < 131) {
                            near = true;
                        }
                    }
                    if (near) {
                        if (boss.getScale().x > 1) boss.setPosition(ground.getPosition().x + 1722, 250);
                        else boss.setPosition(ground.getPosition().x + 227, 138);

                        if (timer_boss < 0) {
                            boss.setTextureRect(IntRect(i_boss * 140, 372, 140, 93));
                            i_boss++;
                            timer_boss = 0.09;
                        }
                        else {
                            timer_boss -= deltaTime;
                        }
                        if (i_boss > 9) {
                            boss.scale(-1, 1);
                            E_COUNTER = 2.1;
                            i_boss = 9;
                            near = false;
                        }
                    }
                }
                else near = false;
            }
            else {
                if (joined == 0) {
                    E_COUNTER = 0;
                    i_boss = 0;
                    joined = 1;
                }
                if (E_COUNTER >= 0 && E_COUNTER <= 1.2) {
                    knife.stop();
                    if (boss.getScale().x > 1) boss.setPosition(ground.getPosition().x + 1722, 250);
                    else boss.setPosition(ground.getPosition().x + 227, 138);
                    if (timer_boss < 0) {
                        boss.setTextureRect(IntRect(i_boss * 140, 372, 140, 93));
                        i_boss++;
                        timer_boss = 0.09;
                    }
                    else {
                        timer_boss -= deltaTime;
                    }
                }
                else if (E_COUNTER > 1.2 && E_COUNTER < 2.4) {
                    knife.stop();
                    if (bro.getPosition().x > boss.getPosition().x)
                        boss.setScale(-2, 2);
                    else
                        boss.setScale(2, 2);

                    boss.setPosition(ground.getPosition().x + 570, 480);

                    if (timer_boss < 0) {
                        boss.setTextureRect(IntRect(i_boss * 140, 372, 140, 93));
                        i_boss--;
                        timer_boss = 0.09;
                    }
                    else {
                        timer_boss -= deltaTime;
                    }
                    if (i_boss == 0) E_COUNTER = 2.4;
                }
                else if (!bro.getGlobalBounds().intersects(boss_hitBox.getGlobalBounds())) {
                    knife.stop();
                    boss.move(-pos_right_D.x * sizes, -pos_right_D.y * sizes);
                    boss.move(-pos_left_A.x * sizes, -pos_left_A.y * sizes);
                    if (!boss_AFK || abs(boss_hitBox.getPosition().x - bro.getPosition().x) < 223) {
                        if (timer_boss < 0) {
                            i_boss++;
                            i_boss %= 8;
                            boss.setTextureRect(IntRect(i_boss * 140, 93, 140, 93));
                            timer_boss = 0.09;
                        }
                        else {
                            timer_boss -= deltaTime;
                        }
                        joined = 1;
                        traps = false;
                    }
                    else {
                        if (joined == 1) {
                            i_boss = 0;
                            joined = 2;
                        }
                        if (timer_boss < 0) {
                            i_boss %= 9;
                            boss.setTextureRect(IntRect(i_boss * 140, 465, 140, 93));
                            i_boss++;
                            timer_boss = 0.09;
                        }
                        else {
                            timer_boss -= deltaTime;
                        }
                        traps = true;
                    }
                    if (bro.getPosition().x > boss.getPosition().x) {
                        boss.move(boss_speed * deltaTime, 0);;
                        boss.setScale(-2, 2);
                    }
                    else {
                        boss.move(-boss_speed * deltaTime, 0);;
                        boss.setScale(2, 2);
                    }

                    //boss collision
                    if (bossBounds.intersects(rect5Bounds)) {
                        Vector2f correctionVector2;
                        float xOverlap = min(bossBounds.left + bossBounds.width - rect5Bounds.left, rect5Bounds.left + rect5Bounds.width - bossBounds.left);
                        float yOverlap = min(bossBounds.top + bossBounds.height - rect5Bounds.top, rect5Bounds.top + rect5Bounds.height - bossBounds.top);
                        if (xOverlap < yOverlap) {
                            correctionVector2.x = (bossBounds.left < rect5Bounds.left) ? -xOverlap : xOverlap;
                        }
                        else {
                            correctionVector2.y = (bossBounds.top < rect5Bounds.top) ? -yOverlap : yOverlap;
                        }
                        boss.move(correctionVector2 * deltaTime);
                        boss_AFK = true;
                    }
                    else if (bossBounds.intersects(rect4Bounds)) {
                        Vector2f correctionVector2;
                        float xOverlap = min(bossBounds.left + bossBounds.width - rect4Bounds.left, rect4Bounds.left + rect4Bounds.width - bossBounds.left);
                        float yOverlap = min(bossBounds.top + bossBounds.height - rect4Bounds.top, rect4Bounds.top + rect4Bounds.height - bossBounds.top);
                        if (xOverlap < yOverlap) {
                            correctionVector2.x = (bossBounds.left < rect4Bounds.left) ? -xOverlap : xOverlap;
                        }
                        else {
                            correctionVector2.y = (bossBounds.top < rect4Bounds.top) ? -yOverlap : yOverlap;
                        }
                        boss.move(correctionVector2 * deltaTime);
                        boss_AFK = true;
                    }
                    else {
                        boss_AFK = false;
                    }

                }
                else {
                    //damage here
                    intersects = true;
                    if (knife.getStatus() == Music::Stopped) knife.play();
                    if (timer_boss < 0) {
                        boss.setTextureRect(IntRect(i_boss * 140, 186, 140, 93));
                        i_boss++;
                        i_boss %= 10;
                        timer_boss = 0.09;
                    }
                    else {
                        timer_boss -= deltaTime;
                    }
                }
                if (traps) {
                    if (broBounds.intersects(rect7Bounds)) {
                        boss_magic.setPosition(bro.getPosition().x - 119, 117);
                        if (timer_magic_boss < 0) {
                            boss_magic.setTextureRect(IntRect(i_magic_boss * 140, 558, 140, 93));
                            i_magic_boss++;
                            i_magic_boss %= 8;
                            timer_magic_boss = 0.09;
                        }
                        else {
                            timer_magic_boss -= deltaTime;
                        }
                    }
                    else if (broBounds.intersects(rect6Bounds)) {
                        boss_magic.setPosition(bro.getPosition().x - 119, 230);
                        if (timer_magic_boss < 0) {
                            boss_magic.setTextureRect(IntRect(i_magic_boss * 140, 558, 140, 93));
                            i_magic_boss++;
                            i_magic_boss %= 8;
                            timer_magic_boss = 0.09;
                        }
                        else {
                            timer_magic_boss -= deltaTime;
                        }
                    }
                    else {
                        boss_magic.setTextureRect(IntRect(0, 0, 0, 0));
                    }
                }
                else {
                    boss_magic.setTextureRect(IntRect(0, 0, 0, 0));
                }
            }

            //fire balls
            if (COUNTER > 7 && COUNTER < animation1_time + 9) {
                if (nyzak.getStatus() == Music::Stopped) nyzak.play();
                for (int i = 0; i < 8; i++) {
                    if (timer3 < 0) {
                        fireballs[i].setTextureRect(IntRect((fireballs_iterator * 300) + 20, 0, 300, 375));
                        fireballs_iterator++;
                        fireballs_iterator %= 8;
                        timer3 = 0.12;
                    }
                    else {
                        timer3 -= deltaTime;
                    }
                    if (bro.getGlobalBounds().intersects(fireballs[i].getGlobalBounds()))
                        ball_intersect = true;
                    if (fireballs[i].getPosition().y > 1080) {
                        if (COUNTER < animation1_time - 1)
                            fireballs[i].setPosition(rand() % 1652, -140);
                        else {
                            fireballs[i].setTextureRect(IntRect(0, 0, 0, 0));
                            nyzak.stop();
                        }
                    }
                    fireballs[i].move(0, speed_fireballs * deltaTime);
                }
            }
            else {
                nyzak.stop();
            }

            //timer display
            if (timer6 < 0) {
                if (TIME_Display > 0) TIME_Display--;
                if (TIME_Display <= 20) {
                    Time.setTexture(timer_T[1]);
                    if (end_of_game.getStatus() == Music::Stopped) end_of_game.play();
                }
                t++;
                t %= 5;
                Time.setTextureRect(IntRect(t * 490, 0, 490, 348));
                TIME.setString(to_string(TIME_Display));
                if (hard) timer6 = 1.1;
                else timer6 = 1.5;
            }
            else {
                timer6 -= deltaTime;
            }

            //ability movement
            if (timer7 < 0) {
                pyramid.setTextureRect(IntRect(p * 500, 0, 500, 475));
                p++;
                p %= 11;
                timer7 = 0.03;
            }
            else {
                timer7 -= deltaTime;
            }

            //player damage
            if (bro.getGlobalBounds().intersects(boss_magic.getGlobalBounds())) {
                intersects = true;
                magic_intersects = true;
            }
            if ((intersects && !ability_2 && COUNTER > 2) || ball_intersect) {
                if (!grounded) { jump.stop(); }
                if (hurt.getStatus() == Music::Stopped && COUNTER > 2) {
                    hurt.play();
                }
                bro.setTexture(player_d);
                if (timer5 < 0) {
                    timer5 = 0.4;

                    if (!magic_intersects) {
                        if (hard) {
                            health[1].scale(0.68, 1);
                            if (health[1].getScale().x < 0.33)
                                health[1].scale(0.6, 1);
                        }
                        else {
                            health[1].scale(0.78, 1);
                            if (health[1].getScale().x < 0.33)
                                health[1].scale(0.7, 1);
                        }
                    }
                    else {
                        if (hard) {
                            health[1].scale(0.65, 1);
                        }
                        else {
                            health[1].scale(0.75, 1);
                        }
                    }
                }
                else {
                    timer5 -= deltaTime;
                }
            }
            else {
                hurt.stop();
                bro.setTexture(player_t);
            }

            //hearts
            if (health[1].getScale().x <= 0.003) {
                if (heart_num < 3) {
                    you_are_dead[rand() % 2].play();
                    lvl4.stop();
                    ii = 4;
                    bro.setTexture(player_dd);
                    bro.setPosition(bro.getPosition().x - 70, bro.getPosition().y - 58);
                    while (ii < 9) {
                        if (timer < 0) {
                            bro.setScale(2, 2);
                            bro.setTextureRect(IntRect(ii * 92, 0, 92, 102));
                            ii++;
                            timer = 0.07;
                        }
                        else {
                            timer -= deltaTime;
                        }
                        window.setView(view);
                        window.clear();
                        window.draw(background);
                        for (int i = 0; i < 4; i++) window.draw(ability[i]);
                        window.draw(ground);
                        window.draw(bro);
                        window.draw(boss);
                        window.draw(boss_magic);
                        for (int i = 0; i < 8; i++) window.draw(fireballs[i]);
                        window.draw(ground2);
                        if (ability_2) window.draw(shield);
                        window.draw(health[0]);
                        window.draw(health[1]);
                        window.draw(pyramid);
                        for (int i = 0; i < 3; i++) window.draw(heart[i]);
                        window.draw(health[2]);
                        for (int i = 0; i < 4; i++) window.draw(Slots[i]);
                        window.draw(slot);
                        window.draw(Time);
                        window.draw(TIME);
                        if (COUNTER > animation1_time + 1) {
                            for (int i = 0; i < 3; i++) window.draw(boss_Hbar[i]);
                        }
                        if (!replay_lvl4) {
                            window.draw(shape99);
                            window.draw(syntax);
                        }
                        window.display();
                    }
                    ii = 0;
                    heart_num++;
                    health[1].setScale(0.266 * 0.86051041667, 0.266 * 0.86051041667);
                    bro.setPosition(bro.getPosition().x + 70, -400);
                }
                else {
                    dead = true;
                }
            }
            for (int i = 0; i < heart_num && heart_num > 0; i++) {
                if (timer11 < 0) {
                    heart[i].setTextureRect(IntRect(heart_anime * 125.1, 0, 125.1, 126));
                    if (i == (heart_num - 1)) {
                        heart_anime %= 10;
                        heart_anime++;
                        timer11 = 0.09;
                    }
                    else if (heart_num == 1) {
                        heart_anime++;
                        heart_anime %= 9;
                        timer11 = 0.09;
                    }
                }
                else {
                    if (heart_num > 1)
                        i = (heart_num - 1);
                    timer11 -= deltaTime;
                }
                if (heart_anime == 9) {
                    for (int j = 0; j < heart_num && heart_num > 0; j++) {
                        heart[j].setScale(0, 0);
                    }
                }
            }

            //boss die
            for (int i = 0; i < ammos.size(); i++) {
                if (ammos[i].getGlobalBounds().intersects(boss_hitBox.getGlobalBounds()) && COUNTER > animation1_time + 2) {
                    if (ammos[i].getTexture() == &ammoT[0]) {
                        if (!hard) {
                            boss_Hbar[1].scale(0.97, 1);
                            if (boss_Hbar[1].getScale().x < 0.38) boss_Hbar[1].scale(0.99, 1);
                        }
                        else {
                            boss_Hbar[1].scale(0.98, 1);
                            if (boss_Hbar[1].getScale().x < 0.38) boss_Hbar[1].scale(0.99, 1);
                        }
                    }
                    else {
                        if (!hard) {
                            boss_Hbar[1].scale(0.94, 1);
                            if (boss_Hbar[1].getScale().x < 0.38) boss_Hbar[1].scale(0.98, 1);
                        }
                        else {
                            boss_Hbar[1].scale(0.975, 1);
                            if (boss_Hbar[1].getScale().x < 0.38) boss_Hbar[1].scale(0.99, 1);
                        }
                    }
                    ammos[i].setScale(0, 0);
                    if (punch.getStatus() == Music::Stopped) punch.play();
                }
                if (ammos[i].getGlobalBounds().intersects(boss_hitBox.getGlobalBounds()) && COUNTER > animation1_time + 2 && intersects && Mouse::isButtonPressed(Mouse::Left) && (Mouse::getPosition(window).x - boss.getPosition().x > 0)) {
                    if (ammos[i].getTexture() == &ammoT[0]) {
                        if (!hard) {
                            boss_Hbar[1].scale(0.97, 1);
                            if (boss_Hbar[1].getScale().x < 0.38) boss_Hbar[1].scale(0.99, 1);
                        }
                        else {
                            boss_Hbar[1].scale(0.98, 1);
                            if (boss_Hbar[1].getScale().x < 0.38) boss_Hbar[1].scale(0.99, 1);
                        }
                    }
                    else {
                        if (!hard) {
                            boss_Hbar[1].scale(0.94, 1);
                            if (boss_Hbar[1].getScale().x < 0.38) boss_Hbar[1].scale(0.98, 1);
                        }
                        else {
                            boss_Hbar[1].scale(0.975, 1);
                            if (boss_Hbar[1].getScale().x < 0.38) boss_Hbar[1].scale(0.99, 1);
                        }
                    }
                    ammos[i].setScale(0, 0);
                    if (punch.getStatus() == Music::Stopped) punch.play();
                }
                else if (ammos[i].getGlobalBounds().intersects(boss_hitBox.getGlobalBounds()) && COUNTER > animation1_time + 2 && intersects && Mouse::isButtonPressed(Mouse::Left) && (Mouse::getPosition(window).x - boss.getPosition().x <= 0)) {
                    if (ammos[i].getTexture() == &ammoT[0]) {
                        if (!hard) {
                            boss_Hbar[1].scale(0.97, 1);
                            if (boss_Hbar[1].getScale().x < 0.38) boss_Hbar[1].scale(0.99, 1);
                        }
                        else {
                            boss_Hbar[1].scale(0.98, 1);
                            if (boss_Hbar[1].getScale().x < 0.38) boss_Hbar[1].scale(0.99, 1);
                        }
                    }
                    else {
                        if (!hard) {
                            boss_Hbar[1].scale(0.94, 1);
                            if (boss_Hbar[1].getScale().x < 0.38) boss_Hbar[1].scale(0.98, 1);
                        }
                        else {
                            boss_Hbar[1].scale(0.975, 1);
                            if (boss_Hbar[1].getScale().x < 0.38) boss_Hbar[1].scale(0.99, 1);
                        }
                    }
                    ammos[i].setScale(0, 0);
                    if (punch.getStatus() == Music::Stopped) punch.play();
                }
            }

            //ability pos
            if (int(C_COUNTER) == 15 && ability_number < 4) {
                C_COUNTER = 0;
                ability[ability_number].setPosition(800, -100);
                ability_number++;
            }
            for (int i = 0; i < 4; i++) {
                if (ability[i].getPosition().x < 5000) {
                    ability[i].move(0, 45 * deltaTime);
                }
                if (ability[i].getPosition().y > 700) {
                    ability[i].setScale(0, 0);
                }
            }

            //ability collect
            if (bro.getGlobalBounds().intersects(ability[0].getGlobalBounds())) {
                abilities_collecte++;
                ability_1 = true; //fire ball
                if (abilities_collecte == 1)
                    Slots[0].setPosition(138, 10 + 68);
                else if (abilities_collecte == 2)
                    Slots[0].setPosition(138 + 77, 10 + 68);
                else if (abilities_collecte == 3)
                    Slots[0].setPosition(138 + 77 + 77, 10 + 68);
                else if (abilities_collecte == 4)
                    Slots[0].setPosition(138 + 77 + 77 + 77, 10 + 68);
            }
            else if (bro.getGlobalBounds().intersects(ability[1].getGlobalBounds())) {
                abilities_collecte++;
                ability_2 = true; //shield
                if (abilities_collecte == 1)
                    Slots[1].setPosition(138, 10 + 68);
                else if (abilities_collecte == 2)
                    Slots[1].setPosition(138 + 77, 10 + 68);
                else if (abilities_collecte == 3)
                    Slots[1].setPosition(138 + 77 + 77, 10 + 68);
                else if (abilities_collecte == 4)
                    Slots[1].setPosition(138 + 77 + 77 + 77, 10 + 68);
            }
            else if (bro.getGlobalBounds().intersects(ability[2].getGlobalBounds())) {
                abilities_collecte++;
                ability_3 = true; //health
                if (abilities_collecte == 1)
                    Slots[2].setPosition(138, 10 + 68);
                else if (abilities_collecte == 2)
                    Slots[2].setPosition(138 + 77, 10 + 68);
                else if (abilities_collecte == 3)
                    Slots[2].setPosition(138 + 77 + 77, 10 + 68);
                else if (abilities_collecte == 4)
                    Slots[2].setPosition(138 + 77 + 77 + 77, 10 + 68);
            }
            else if (bro.getGlobalBounds().intersects(ability[3].getGlobalBounds())) {
                abilities_collecte++;
                ability_4 = true; //speed
                if (abilities_collecte == 1)
                    Slots[3].setPosition(138, 10 + 68);
                else if (abilities_collecte == 2)
                    Slots[3].setPosition(138 + 77, 10 + 68);
                else if (abilities_collecte == 3)
                    Slots[3].setPosition(138 + 77 + 77, 10 + 68);
                else if (abilities_collecte == 4)
                    Slots[3].setPosition(138 + 77 + 77 + 77, 10 + 68);
            }

            //abilitys
            for (int i = 0; i < 4; i++) {
                if (bro.getGlobalBounds().intersects(ability[i].getGlobalBounds())) {
                    if (ability_s.getStatus() == Music::Stopped) ability_s.play();
                    ability[i].setScale(0, 0);
                }
                slot.setTexture(slot_T[abilities_collecte]);
            }
            if (ability_1) {
                for (int i = 0; i < 2; i++)
                    ammo[i].setTexture(ammoT[1]);
            }
            if (ability_2 && COUNTER_ABILITY2 <= 8) {
                health[1].setScale(health[1].getScale().x, health[1].getScale().y);
            }
            else if (COUNTER_ABILITY2 > 8) {
                ability_2 = false;
            }
            if (ability_3) {
                health[1].setScale(0.266 * 0.86051041667, 0.266 * 0.86051041667);
                ability_3 = false;
            }

            if (timer_count < 0) {
                if (ability_2) {
                    COUNTER_ABILITY2 += 0.1;
                }
                if (ability_4) {
                    COUNTER_ABILITY4 += 0.1;
                }
                COUNTER += 0.1;
                C_COUNTER += 0.1;
                E_COUNTER += 0.1;
                timer_count = 0.1;
            }
            else {
                timer_count -= deltaTime;
            }

            shape99.setSize(Vector2f(syntax.getGlobalBounds().width + 10, syntax.getGlobalBounds().height + 10));
            shape99.setOrigin((syntax.getGlobalBounds().width / 2.1) + 5, syntax.getGlobalBounds().height / 1.68);
            shape99.setPosition(syntax.getPosition());

            if (!replay_lvl4) {
                if (COUNTER > 1 && COUNTER <= 3)
                    text = "You have reached the final level";
                else if (COUNTER > 3 && COUNTER <= 5)
                    text = "the boss has magical abilities, so be careful";
                else if (COUNTER > 5 && COUNTER <= 6)
                    text = "(Stefan) Don't worry, I....";
                else if (COUNTER > 6 && COUNTER <= 8)
                    text = "(The Boss) Do you think anyone can help you?";
                else if (COUNTER > 8 && COUNTER <= 10)
                    text = "you are mine now";
                else if (COUNTER > 10 && COUNTER <= 12)
                    text = "I burned a whole city in 1864";
                else if (COUNTER > 12 && COUNTER <= 14)
                    text = "What can a little boy do?";
                else if (COUNTER > 14 && COUNTER <= 16)
                    text = "(Stefan) You will be surprised";
                else {
                    text = "";
                    shape99.setSize(Vector2f(0, 0));
                }
            }
            else {
                text = "";
                shape99.setSize(Vector2f(0, 0));
            }
            syntax.setString(text);
            syntax.setPosition(1920 / 2, 960);
            syntax.setOrigin(syntax.getGlobalBounds().width / 2.1, syntax.getGlobalBounds().height / 1.68);

            shield.setPosition(bro.getPosition().x + bro.getTextureRect().width / 2 + 14, bro.getPosition().y + bro.getTextureRect().height / 2 + 25);

            window.setView(view);
            if (TIME_Display <= 20) {
                if (shakeClock.getElapsedTime().asSeconds() < shakeTime) {
                    float offsetX = (rand() % (int)(shakeIntensity * 2)) - shakeIntensity;
                    float offsetY = (rand() % (int)(shakeIntensity * 2)) - shakeIntensity;
                    View shakeView = view;
                    shakeView.move(offsetX, offsetY);
                    window.setView(shakeView);
                }
                else {
                    window.setView(window.getDefaultView());
                    shakeClock.restart();
                }
            }
            if (COUNTER > 0) {
                window.clear();
                window.draw(background);
                for (int i = 0; i < 4; i++) window.draw(ability[i]);
                window.draw(ground);
                window.draw(bro);
                window.draw(boss);
                window.draw(boss_magic);
                for (int i = 0; i < 8; i++) window.draw(fireballs[i]);
                window.draw(ground2);
                if (ability_2) window.draw(shield);
                for (int i = 0; i < ammos.size(); i++) window.draw(ammos[i]);
                window.draw(health[0]);
                window.draw(health[1]);
                window.draw(pyramid);
                for (int i = 0; i < 3; i++) window.draw(heart[i]);
                window.draw(health[2]);
                for (int i = 0; i < 4; i++) window.draw(Slots[i]);
                window.draw(slot);
                window.draw(Time);
                window.draw(TIME);
                if (COUNTER > animation1_time + 1) {
                    for (int i = 0; i < 3; i++) window.draw(boss_Hbar[i]);
                    if (ability_4 && COUNTER_ABILITY4 <= 8) {
                        speed = 500;
                        jumphight = 34;
                    }
                    else if (COUNTER_ABILITY4 > 8) {
                        ability_4 = false;
                    }
                }
                if (!replay_lvl4) {
                    window.draw(shape99);
                    window.draw(syntax);
                }
                //window.draw(boss_hitBox);
                window.display();
            }
            deltaTime = c.getElapsedTime().asSeconds();

            if (dead || TIME_Display == 0) {
                music_bg.stop();
                foot[0].stop();
                foot[1].stop();
                shoot.stop();
                ability_s.stop();
                jump.stop();
                knife.stop();
                punch.stop();
                hurt.stop();
                end_of_game.stop();
                you_are_dead[0].stop();
                you_are_dead[1].stop();
                gate_sound.stop();
                lvl4.stop();
                nyzak.stop();
                game_over_sound.play();
                bro.setTexture(player_dd);
                bro.setPosition(bro.getPosition().x - 70, bro.getPosition().y - 58);
                ii = 4;
                while (ii < 9) {
                    if (timer < 0) {
                        bro.setScale(2, 2);
                        bro.setTextureRect(IntRect(ii * 92, 0, 92, 102));
                        ii++;
                        timer = 0.075;
                    }
                    else {
                        timer -= deltaTime;
                    }
                    window.setView(view);
                    window.clear();
                    window.draw(background);
                    for (int i = 0; i < 4; i++) window.draw(ability[i]);
                    window.draw(ground);
                    window.draw(bro);
                    window.draw(boss);
                    window.draw(boss_magic);
                    for (int i = 0; i < 8; i++) window.draw(fireballs[i]);
                    window.draw(ground2);
                    if (ability_2) window.draw(shield);
                    window.draw(health[0]);
                    window.draw(health[1]);
                    window.draw(pyramid);
                    for (int i = 0; i < 3; i++) window.draw(heart[i]);
                    window.draw(health[2]);
                    for (int i = 0; i < 4; i++) window.draw(Slots[i]);
                    window.draw(slot);
                    window.draw(Time);
                    window.draw(TIME);
                    if (COUNTER > animation1_time + 1) {
                        for (int i = 0; i < 3; i++) window.draw(boss_Hbar[i]);
                    }
                    if (!replay_lvl4) {
                        window.draw(shape99);
                        window.draw(syntax);
                    }
                    window.display();
                }
                PageNumber = -6;
                replay_lvl4 = true;
                window.setView(window.getDefaultView());
                gameOver(window, 5);
                window.setView(view);
                if (PageNumber != 5) {
                    window.setView(window.getDefaultView());
                    return;
                }
            }
        }
    }
}

void winWindow(RenderWindow& window, int windowNum) {
    RectangleShape shape(Vector2f(1920, 1080));
    shape.setFillColor(Color(0, 0, 0, 190));

    Texture game_over_t;
    game_over_t.loadFromFile("./assets/textures/win_menu.png");
    Sprite game_over;
    game_over.setTexture(game_over_t);

    Texture background_T;
    background_T.loadFromFile("./assets/textures/Untitled965 (02).png");
    Sprite background;
    background.setTexture(background_T);

    Font font;
    font.loadFromFile("./assets/font/HeadlinerNo.45 DEMO.ttf");

    Text resumeText("Next", font, 69);
    resumeText.setFillColor(Color::White);
    resumeText.setPosition(118, 570);

    Text quitText("Quit", font, 69);
    quitText.setFillColor(Color::White);
    quitText.setPosition(118, 683);

    Music click;
    click.openFromFile("./assets/sounds/click.mp3");
    click.setVolume(50);
    Music enter;
    enter.openFromFile("./assets/sounds/enter.mp3");
    enter.setVolume(50);

    int selectedOption = 3;
    Clock c;
    float deltaTime = 0, timer = 0;
    int B = 0, B2 = 0;
    bool isMouse = false;
    while (window.isOpen()) {
        if (PageNumber != -7) break;
        c.restart();
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            //mouse
            RectangleShape mouse(Vector2f(15, 20));
            Vector2i mousePosition = Mouse::getPosition(window);
            Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);
            mouse.setPosition(worldMousePosition);

            if (mouse.getGlobalBounds().intersects(resumeText.getGlobalBounds())) {
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    if (windowNum == 2) {
                        enter.play();
                        PageNumber = 2;
                        gameWindow2(window);
                        if (PageNumber != 2) return;
                    }
                    else if (windowNum == 3) {
                        enter.play();
                        PageNumber = 3;
                        gameWindow2(window);
                        if (PageNumber != 3) return;
                    }
                    else if (windowNum == 1) {
                        enter.play();
                        PageNumber = 1;
                        gameWindow1(window);
                        if (PageNumber != 1) return;
                    }
                    else if (windowNum == 4) {
                        enter.play();
                        PageNumber = 4;
                        if (PageNumber != 4) return;
                        sleep(milliseconds(150));
                        return;
                    }
                    else if (windowNum == 5) {
                        enter.play();
                        PageNumber = 5;
                        if (PageNumber != 5) return;
                        sleep(milliseconds(150));
                        gameWindow5(window);
                    }
                }
                selectedOption = 0;
                isMouse = true;
            }
            else if (mouse.getGlobalBounds().intersects(quitText.getGlobalBounds())) {
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    enter.play();
                    PageNumber = -3;
                    loading(window, 1000);
                    return;
                }
                selectedOption = 1;
                isMouse = true;
            }
            else {
                isMouse = false;
            }
            if (event.type == Event::KeyPressed && !isMouse) {
                if (event.key.code == Keyboard::Up) {
                    click.play();
                    selectedOption = (selectedOption - 1 + 2) % 2;
                }
                else if (event.key.code == Keyboard::Down) {
                    click.play();
                    selectedOption = (selectedOption + 1) % 2;
                }
                else if (event.key.code == Keyboard::Enter) {
                    enter.play();
                    if (selectedOption == 0) {
                        if (windowNum == 2) {
                            enter.play();
                            PageNumber = 2;
                            gameWindow2(window);
                            if (PageNumber != 2) return;
                        }
                        else if (windowNum == 3) {
                            enter.play();
                            PageNumber = 3;
                            gameWindow2(window);
                            if (PageNumber != 3) return;
                        }
                        else if (windowNum == 1) {
                            enter.play();
                            PageNumber = 1;
                            gameWindow1(window);
                            if (PageNumber != 1) return;
                        }
                        else if (windowNum == 4) {
                            enter.play();
                            PageNumber = 4;
                            sleep(milliseconds(150));
                            if (PageNumber != 4) return;
                            return;
                        }
                        else if (windowNum == 5) {
                            enter.play();
                            PageNumber = 5;
                            if (PageNumber != 5) return;
                            sleep(milliseconds(150));
                            gameWindow5(window);
                        }
                    }
                    else if (selectedOption == 1) {
                        PageNumber = -3;
                        loading(window, 1000);
                        return;
                    }
                }
            }
        }

        if (timer < 0) {
            background.setTextureRect(IntRect(B * 1920, B2 * 1080, 1920, 1080));
            B++;
            B %= 9;
            if (B == 8) {
                B = 0;
                B2++;
            }
            if (B2 == 7) {
                B2 = 0;
            }
            timer = 0.066;
        }
        else {
            timer -= deltaTime;
        }

        window.clear();
        window.draw(background);
        window.draw(shape);
        window.draw(game_over);
        if (selectedOption == 0) {
            resumeText.setFillColor(Color(38, 83, 186));
            quitText.setFillColor(Color::White);
        }
        else if (selectedOption == 1) {
            resumeText.setFillColor(Color::White);
            quitText.setFillColor(Color(38, 83, 186));
        }
        window.draw(resumeText);
        window.draw(quitText);
        window.display();
        deltaTime = c.getElapsedTime().asSeconds();
    }
}

void gameOver(RenderWindow& window, int windowNum) {
    RectangleShape shape(Vector2f(1920, 1080));
    shape.setFillColor(Color(0, 0, 0, 190));

    Texture game_over_t;
    game_over_t.loadFromFile("./assets/textures/game_over.png");
    Sprite game_over;
    game_over.setTexture(game_over_t);

    Texture background_T;
    background_T.loadFromFile("./assets/textures/Untitled965 (01).png");
    Sprite background;
    background.setTexture(background_T);

    Font font;
    font.loadFromFile("./assets/font/HeadlinerNo.45 DEMO.ttf");

    Text resumeText("RePlay", font, 69);
    resumeText.setFillColor(Color::White);
    resumeText.setPosition(118, 570);

    Text quitText("Quit", font, 69);
    quitText.setFillColor(Color::White);
    quitText.setPosition(118, 683);

    Texture slot_t;
    slot_t.loadFromFile("./assets/textures/slot 0.png");
    Sprite slot;
    slot.setTexture(slot_t);
    slot.setPosition(slot.getPosition().x + 38, slot.getPosition().y + 10);
    slot.setScale(0.266, 0.266);

    Music click;
    click.openFromFile("./assets/sounds/click.mp3");
    click.setVolume(50);
    Music enter;
    enter.openFromFile("./assets/sounds/enter.mp3");
    enter.setVolume(50);

    int selectedOption = 3;
    Clock c;
    float deltaTime = 0, timer = 0;
    int B = 0, B2 = 0;
    bool isMouse = false;
    while (window.isOpen()) {
        if (PageNumber != -6) break;
        c.restart();
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            //mouse
            RectangleShape mouse(Vector2f(15, 20));
            Vector2i mousePosition = Mouse::getPosition(window);
            Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);
            mouse.setPosition(worldMousePosition);

            if (mouse.getGlobalBounds().intersects(resumeText.getGlobalBounds())) {
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    if (windowNum == 2) {
                        enter.play();
                        PageNumber = 2;
                        gameWindow2(window);
                        if (PageNumber != 2) return;
                    }
                    else if (windowNum == 3) {
                        enter.play();
                        PageNumber = 3;
                        gameWindow2(window);
                        if (PageNumber != 3) return;
                    }
                    else if (windowNum == 4) {
                        enter.play();
                        PageNumber = 4;
                        gameWindow4(window, slot);
                        if (PageNumber != 4) return;
                    }
                    else if (windowNum == 1) {
                        enter.play();
                        PageNumber = 1;
                        gameWindow1(window);
                        if (PageNumber != 1) return;
                    }
                    else if (windowNum == 5) {
                        enter.play();
                        PageNumber = 5;
                        gameWindow5(window);
                        if (PageNumber != 5) return;
                    }
                }
                selectedOption = 0;
                isMouse = true;
            }
            else if (mouse.getGlobalBounds().intersects(quitText.getGlobalBounds())) {
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    enter.play();
                    PageNumber = -3;
                    loading(window, 1000);
                    return;
                }
                selectedOption = 1;
                isMouse = true;
            }
            else {
                isMouse = false;
            }
            if (event.type == Event::KeyPressed && !isMouse) {
                if (event.key.code == Keyboard::Up) {
                    click.play();
                    selectedOption = (selectedOption - 1 + 2) % 2;
                }
                else if (event.key.code == Keyboard::Down) {
                    click.play();
                    selectedOption = (selectedOption + 1) % 2;
                }
                else if (event.key.code == Keyboard::Enter) {
                    enter.play();
                    if (selectedOption == 0) {
                        if (windowNum == 2) {
                            PageNumber = 2;
                            gameWindow2(window);
                            if (PageNumber != 2) return;
                        }
                        else if (windowNum == 3) {
                            enter.play();
                            PageNumber = 3;
                            gameWindow2(window);
                            if (PageNumber != 3) return;
                        }
                        else if (windowNum == 4) {
                            enter.play();
                            PageNumber = 4;
                            gameWindow4(window, slot);
                            if (PageNumber != 4) return;
                        }
                        else if (windowNum == 1) {
                            enter.play();
                            PageNumber = 1;
                            gameWindow1(window);
                            if (PageNumber != 1) return;
                        }
                        else if (windowNum == 5) {
                            enter.play();
                            PageNumber = 5;
                            gameWindow5(window);
                            if (PageNumber != 5) return;
                        }
                    }
                    else if (selectedOption == 1) {
                        PageNumber = -3;
                        loading(window, 1000);
                        return;
                    }
                }
            }
        }

        if (timer < 0) {
            background.setTextureRect(IntRect(B * 1920, B2 * 1080, 1920, 1080));
            B++;
            B %= 9;
            if (B == 8) {
                B = 0;
                B2++;
            }
            if (B2 == 7) {
                B2 = 0;
            }
            timer = 0.066;
        }
        else {
            timer -= deltaTime;
        }

        window.clear();
        window.draw(background);
        window.draw(shape);
        window.draw(game_over);
        if (selectedOption == 0) {
            resumeText.setFillColor(Color(255, 60, 60));
            quitText.setFillColor(Color::White);
        }
        else if (selectedOption == 1) {
            resumeText.setFillColor(Color::White);
            quitText.setFillColor(Color(255, 60, 60));
        }
        window.draw(resumeText);
        window.draw(quitText);
        window.display();
        deltaTime = c.getElapsedTime().asSeconds();
    }
}

void pause(RenderWindow& window) {
    int menuSelection = 3;
    int pause_count = 0;
    Texture pause_background;
    pause_background.loadFromFile("./assets/textures/pause_menu.png");
    Sprite p_background;
    p_background.setTexture(pause_background);
    Font font;
    font.loadFromFile("./assets/font/HeadlinerNo.45 DEMO.ttf");
    Text play;
    play.setFont(font);
    play.setString("RESUME");
    play.setFillColor(Color::White);
    play.setCharacterSize(69);
    play.setPosition(118, 570);
    Text quit;
    quit.setFont(font);
    quit.setString("QUIT");
    quit.setFillColor(Color::White);
    quit.setCharacterSize(69);
    quit.setPosition(118, 683);

    Music click;
    click.openFromFile("./assets/sounds/click.mp3");
    click.setVolume(50);
    Music enter;
    enter.openFromFile("./assets/sounds/enter.mp3");
    enter.setVolume(50);
    bool isMouse = false;
    while (window.isOpen()) {
        pause_count++;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            //mouse
            RectangleShape mouse(Vector2f(15, 20));
            Vector2i mousePosition = Mouse::getPosition(window);
            Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);
            mouse.setPosition(worldMousePosition);

            if (mouse.getGlobalBounds().intersects(play.getGlobalBounds())) {
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    enter.play();
                    sleep(milliseconds(150));
                    return;
                }
                menuSelection = 0;
                isMouse = true;
            }
            if (event.type == Event::KeyPressed && pause_count > 5) {
                if (event.key.code == Keyboard::Escape) {
                    return;
                }
            }
            else if (mouse.getGlobalBounds().intersects(quit.getGlobalBounds())) {
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    enter.play();
                    new_user = false;
                    sleep(milliseconds(150));
                    PageNumber = -3;
                    loading(window, 1000);
                    return;
                }
                menuSelection = 1;
                isMouse = true;
            }
            else {
                isMouse = false;
            }
            if (event.type == Event::KeyPressed && !isMouse) {
                if (event.key.code == Keyboard::Up) {
                    click.play();
                    menuSelection = (menuSelection - 1 + 2) % 2;
                }
                else if (event.key.code == Keyboard::Down) {
                    click.play();
                    menuSelection = (menuSelection + 1) % 2;
                }
                else if (event.key.code == Keyboard::Enter && menuSelection < 2) {
                    enter.play();
                    sleep(milliseconds(150));
                    if (menuSelection == 0) {
                        return;
                    }
                    else if (menuSelection == 1) {
                        PageNumber = -3;
                        loading(window, 1000);
                        return;
                    }
                }
            }
        }

        //color option
        if (menuSelection == 0) {
            play.setFillColor(Color(38, 83, 186));
            quit.setFillColor(Color::White);
        }
        else if (menuSelection == 1) {
            play.setFillColor(Color::White);
            quit.setFillColor(Color(38, 83, 186));
        }
        window.draw(p_background);
        window.draw(play);
        window.draw(quit);
        window.display();
    }
}
