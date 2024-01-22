// Pin mappings for the tens-place display
int a1 = A1, b1 = A2, c1 = 5, g1 = 13, f1 = A0, d1 = 6, e1 = 7;

// Pin mappings for the ones-place display
int a2 = A5, b2 = 12, g2 = A3, f2 = A4, c2 = 2, d2 = 3, e2 = 4;

// Common pins for both displays
int a, b, c, d, e, f, g = 0;

// Button and buzzer pins
int BTN_PIN = 8;
int BUZZ = 9;
int BUZZ_SWITCH = 10;

void setup()
{
    Serial.begin(9600);

    // Use unconnected A6 analog pin as randomSeed to create a fairly random session
    randomSeed(analogRead(A6));

    // Set up display pins
    for (int i = 2; i <= 13; i++)
    {
        if (i != 11)
        {
            pinMode(i, OUTPUT);
        }
    }
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);

    // BUTTON
    pinMode(BTN_PIN, INPUT_PULLUP);

    // BUZZ SWITCH
    pinMode(BUZZ_SWITCH, INPUT_PULLUP);

    // BUZZER
    pinMode(BUZZ, OUTPUT);

    // Show "HI" on startup
    tens();
    switchnum(10);
    ones();
    switchnum(11);
}

void loop()
{

    byte buttonState = digitalRead(BTN_PIN);
    byte audioOnState = digitalRead(BUZZ_SWITCH);

    if (buttonState == LOW)
    {
        loading();

        // Generate a random number between 0 and 99
        long randNum = random(99);
        Serial.println(randNum);

        // Set the tens-place digit display
        tens();
        switchnum((randNum / 10) % 10);

        // Set the ones-place digit display
        ones();
        switchnum(randNum % 10);

        // Activate siren if the number is greater than 45 and audio is enabled
        if (randNum > 45 && audioOnState == HIGH)
        {
            siren();
        }
    }
}

void siren()
{
    // Activate siren sound
    for (int j = 0; j < 4; j++)
    {
        for (int i = 700; i < 800; i++)
        {
            tone(BUZZ, i);
            delay(3);
        }
        for (int i = 800; i > 700; i--)
        {
            tone(BUZZ, i);
            delay(3);
        }
        noTone(BUZZ);
    }
}

// Binary patterns for each digit (0-9, H, I)
//   G  F  E  D  C  B  A
int digitPatternsBin[12][7] = {
    {0, 1, 1, 1, 1, 1, 1}, // 0
    {0, 0, 0, 0, 1, 1, 0}, // 1
    {1, 0, 1, 1, 0, 1, 1}, // 2
    {1, 0, 0, 1, 1, 1, 1}, // 3
    {1, 1, 0, 0, 1, 1, 0}, // 4
    {1, 1, 0, 1, 1, 0, 1}, // 5
    {1, 1, 1, 1, 1, 0, 1}, // 6
    {0, 0, 0, 0, 1, 1, 1}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 0, 1, 1, 1, 1}, // 9
    {1, 1, 1, 0, 1, 1, 0}, // H
    {0, 1, 1, 0, 0, 0, 0}, // I
};

void switchnum(int number)
{
    // Display the digit based on the numbers variable
    int pins[7] = {g, f, e, d, c, b, a};

    for (int p = 0; p < 7; p++)
    {
        digitalWrite(pins[p], digitPatternsBin[number][p]);
    }
}

void ones()
{
    // Set display pins for ones digit
    a = a2;
    b = b2;
    c = c2;
    d = d2;
    e = e2;
    f = f2;
    g = g2;
}

void tens()
{
    // Set display pins for tens digit
    a = a1;
    b = b1;
    c = c1;
    d = d1;
    e = e1;
    f = f1;
    g = g1;
}

struct SnakeStep
{
    int on[2];
    int off[2];
};
SnakeStep steps[8] = {
    {{d1, d2}, {f1, b2}},
    {{e1, c2}, {g1, g2}},
    {{g1, g2}, {e1, c2}},
    {{b1, f2}, {d1, d2}},
    {{a1, a2}, {e1, c2}},
    {{f1, b2}, {g1, g2}},
    {{g1, g2}, {b1, f2}},
    {{c1, e2}, {a1, a2}},
};

void loading()
{
    clear();

    // Mirrored Snake Animation
    for (int i = 0; i <= 4; i++)
    {

        for (int j = 0; j < 8; j++)
        {
            on(steps[j].on[0], steps[j].on[1]);

            // Turn off specific segments on certain steps
            if (i > 0 || j >= 4)
            {
                off(steps[j].off[0], steps[j].off[1]);
            }

            delay(75);
        }
    }

    // Clear all segments after the animation
    clear();
}

void on(int pin1, int pin2)
{
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
}

void off(int pin1, int pin2)
{
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
}

void clear()
{
    digitalWrite(a1, LOW);
    digitalWrite(b1, LOW);
    digitalWrite(c1, LOW);
    digitalWrite(d1, LOW);
    digitalWrite(e1, LOW);
    digitalWrite(f1, LOW);
    digitalWrite(g1, LOW);
    digitalWrite(a2, LOW);
    digitalWrite(b2, LOW);
    digitalWrite(c2, LOW);
    digitalWrite(d2, LOW);
    digitalWrite(e2, LOW);
    digitalWrite(f2, LOW);
    digitalWrite(g2, LOW);
}
