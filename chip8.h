#ifndef CHIP8_H
#define CHIP8_H

#include <QObject>
#include <QThread>
#include <QByteArray>
#include <QFile>
#include <QTimer>

#define MEM_SIZE 4096   // Memory size
#define REG_SIZE 16     // Register size
#define STA_SIZE 16     // Stack size

class Chip8 : public QThread
{
    Q_OBJECT
public:
    explicit Chip8(QObject *parent = 0);
    ~Chip8();

    QTimer drawTimer;

    void run() Q_DECL_OVERRIDE;

    // This will hold pixel situation
    unsigned char gfx[64 * 32];

    unsigned char gfxSync[64 * 32];
    int gfxState[64 * 32];
    int syncval = 0;
    bool syncgraphic = false;

    // Current working code
    unsigned short opcode;

    // Memory
    unsigned char memory[MEM_SIZE];

    // V will hold the registers
    unsigned char V[REG_SIZE];

    // Program counter
    unsigned short pc;

    // There are 2 timers in Chip8
    unsigned char delay_timer;
    unsigned char sound_timer;

    // Stack
    unsigned short stack[16];

    // General purpose register
    unsigned short I;

    // Stack pointer
    unsigned short sp;

    // Keys will be hold with this array
    unsigned char key[16];

    void initialize();
    bool loadGame(QString path);

    void startEmulation();
    void stopEmulation();

    void emulateCycle();

    void decodeOpCode();

    void setKeyPressed(int index);
    void setKeyReleased(int index);

    bool drawFlag() const;
    void setDrawFlag(bool drawFlag);

    bool running() const;
    void setRunning(bool running);

    QTimer *getDelayTimer() const;
    void setDelayTimer(QTimer *value);

private:
    int _opcodes;
    bool _drawFlag;
    bool _running = false;

    QTimer *delayTimer;
    QTimer *soundTimer;

    short opcodeX();
    short opcodeY();
    short opcodeN();
    short opcodeNN();
    short opcodeNNN();

    void nextInstruction();
    void skipNextInstruction();

    void handleTimers();

signals:
    void resultReady(const QString &s);
    void drawScreen();

public slots:
    void updateDelayTimer();
};

#endif // CHIP8_H
