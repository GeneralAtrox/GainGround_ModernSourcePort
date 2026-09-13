#pragma once
#include <array>
#include <cstdint>
#include <span>
#include <vector>

namespace gain_ground::gameplay {
struct NavigationPoint { double x{}, y{}; };
struct NavigationBody {
    unsigned id{};
    NavigationPoint position;
    double half_width{10}, half_height{9};
};
struct NavigationReservation { unsigned id{}; NavigationPoint position; };
class NavigationWorld {
public:
    virtual ~NavigationWorld() = default;
    // Includes the mover's footprint and its terrain/height restrictions.
    virtual bool walkable(NavigationPoint) const = 0;
    // Project pursuit into the actor's permitted rectangle, not through terrain.
    virtual NavigationPoint constrain_goal(NavigationPoint goal) const { return goal; }
    virtual std::span<const NavigationBody> bodies() const = 0;
    virtual std::span<const NavigationReservation> reservations() const { return {}; }
};
struct NavigationState {
    NavigationPoint previous{}, goal{};
    std::vector<NavigationPoint> route;
    unsigned stalled{}, age{};
    bool initialized{}, navigating{}, boundary_goal{};
    NavigationPoint waiting_position{}, boundary_anchor{}, progress_point{}, yield_direction{};
    double best_remaining{1e30};
    unsigned blocked_updates{}, no_progress{}, yield_updates{};
    bool waiting_assigned{}, waiting{}, dynamic_route{};
};
class EnemyNavigation {
public:
    NavigationPoint steer(NavigationState &, const NavigationWorld &,
                          const NavigationBody &, NavigationPoint goal,
                          NavigationPoint intended) const;
    static bool separates(const NavigationBody &, NavigationPoint delta,
                          const NavigationBody &);
};

// Live RAM adapter for the original ground-walking/contact pipeline. Paths are
// derived state; guest records remain the authority for positions and behavior.
class LegacyEnemyNavigation {
public:
    void reset();
    void prepare(std::span<std::uint8_t> local, std::span<const std::uint8_t> shared,
                 std::uint32_t record, std::uint64_t frame);
    bool separating(std::span<const std::uint8_t> local, std::uint32_t record,
                    std::uint32_t other, std::uint64_t frame) const;
    void record_contact_block(std::span<const std::uint8_t> local, std::uint32_t record,
                              std::uint32_t other, std::uint64_t frame);
    void finish_move(std::span<std::uint8_t> local, std::uint32_t record, std::uint64_t frame);
    std::uint16_t walking_heading(std::span<const std::uint8_t> local, std::uint32_t record,
                                 std::uint64_t frame, std::uint16_t original) const;
    std::uint16_t walking_advance(std::span<const std::uint8_t> local, std::uint32_t record,
                                 std::uint64_t frame, std::uint16_t original) const;
private:
    struct Slot {
        NavigationState state;
        std::uint32_t callback{}, descriptor{};
        std::uint64_t frame{UINT64_MAX};
        bool prepared{}, contact_blocked{};
        bool restore_intent{};
        NavigationPoint held_intent{};
        unsigned facing{}, candidate_facing{}, facing_samples{};
        bool facing_initialized{};
        bool presentation{}, moving{};
        std::uint64_t contact_frame{UINT64_MAX};
        std::uint32_t contact_other{}, contact_callback{}, contact_descriptor{};
    };
    std::array<Slot,128> slots_{};
};
} // namespace gain_ground::gameplay
