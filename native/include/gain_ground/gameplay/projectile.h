#pragma once

namespace gain_ground::gameplay {

enum class ProjectileProbe { clear, expired, stopped };

class ProjectileRuntime {
public:
    virtual ~ProjectileRuntime() = default;
    virtual ProjectileProbe check_bounds() = 0;
    virtual bool advance_lifetime() = 0;
    virtual ProjectileProbe check_contacts() = 0;
    virtual void move_xy() = 0;
    virtual void move_height() = 0;
    virtual void expire() = 0;
    virtual void publish() = 0;
};

// Projectiles have their own behavior family; they are not characters.
class Projectile {
public:
    virtual ~Projectile() = default;
    void update(ProjectileRuntime &) const;
protected:
    virtual bool advance_lifetime(ProjectileRuntime &) const { return true; }
    virtual void move(ProjectileRuntime &runtime) const { runtime.move_xy(); }
};

class TimedProjectile final : public Projectile {
protected:
    bool advance_lifetime(ProjectileRuntime &runtime) const override {
        return runtime.advance_lifetime();
    }
};

class BallisticProjectile final : public Projectile {
protected:
    void move(ProjectileRuntime &runtime) const override {
        runtime.move_xy();
        runtime.move_height();
    }
};

} // namespace gain_ground::gameplay
