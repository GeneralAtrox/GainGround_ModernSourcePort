#include "gain_ground/gameplay/projectile.h"

namespace gain_ground::gameplay {

void Projectile::update(ProjectileRuntime &runtime) const
{
    const auto bounds = runtime.check_bounds();
    if (bounds == ProjectileProbe::stopped) return;
    if (bounds == ProjectileProbe::expired || !advance_lifetime(runtime)) {
        runtime.expire();
        return;
    }
    const auto contacts = runtime.check_contacts();
    if (contacts == ProjectileProbe::stopped) return;
    if (contacts == ProjectileProbe::expired) {
        runtime.expire();
        return;
    }
    move(runtime);
    runtime.publish();
}

} // namespace gain_ground::gameplay
