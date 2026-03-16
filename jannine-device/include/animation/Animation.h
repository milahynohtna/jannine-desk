#pragma once

#include <vector>
#include <cstdint>
#include "images/ImageDescriptor.h"

struct FrameDuration {
    const ImageDescriptor& frame;
    uint16_t durationMs;
};

class Animation {
public:
    virtual ~Animation() = default;


    void tick();
    const ImageDescriptor& currentFrame() const;
    void reset();
    bool isFinished() const;

    // Engine API
    void requestFinish();

protected:
    void setIntro(std::vector<FrameDuration> frames);
    void setLoop(std::vector<FrameDuration> frames);
    void setOutro(std::vector<FrameDuration> frames);
    void stopLoop();

private:
    enum class Phase { Intro, Loop, Outro, Finished };

    Phase _phase = Phase::Intro;
    std::vector<FrameDuration> _intro;
    std::vector<FrameDuration> _loop;
    std::vector<FrameDuration> _outro;

    const std::vector<FrameDuration>* _active = nullptr;
    size_t _index = 0;
    uint32_t _elapsedMs = 0;
    bool _loopEnabled = true;
    uint32_t _lastTickMs = 0;

    void advanceFrame();
    void update(uint32_t deltaMs);


    // helper array untuk transisi generik
    const std::vector<FrameDuration>* getPhaseFrames(Phase p) const;
};
