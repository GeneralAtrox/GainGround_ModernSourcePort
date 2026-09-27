// Stage select: during play the remaining-enemy count at 0xc14 is zeroed once
// so the game's own all-enemies-defeated clear runs (that clear carries the
// players on the field into the next roster), and that clear is routed through
// the new-round branch so CPU A receives both the world bank and the chosen
// stage. Boot, attract and later clears keep the original values. The host
// decisions are exercised directly; the full flow runs the original stage
// teardown against hardware and is covered by scripts/Test-StageSelect.ps1.
#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
void check(bool ok,const char *message){if(!ok)throw std::runtime_error(message);}
std::uint16_t word(std::span<const std::uint8_t> r,unsigned a){return static_cast<std::uint16_t>((r[a]<<8)|r[a+1]);}
constexpr std::uint32_t kController=0x3400; // Stage controller record (A5) in these boards.
struct Board {
    gain_ground::RuntimeHost host;gain_ground::System24Devices devices;
    Board(std::uint16_t stage,std::uint16_t bank,std::uint8_t demo_players=0,std::uint16_t enemies=16){
        host.attach_devices(devices);host.select_cpu(1);
        std::vector<std::uint8_t> ram(0x40000),shared(0x40000);
        ram[0xc00]=static_cast<std::uint8_t>(bank>>8);ram[0xc01]=static_cast<std::uint8_t>(bank);
        ram[0xc02]=static_cast<std::uint8_t>(stage>>8);ram[0xc03]=static_cast<std::uint8_t>(stage);
        ram[0xc06]=demo_players;
        ram[0xc14]=static_cast<std::uint8_t>(enemies>>8);ram[0xc15]=static_cast<std::uint8_t>(enemies);
        host.load_region(2,0,ram);host.load_region(3,0,shared);
    }
    std::uint16_t ram(unsigned a){return word(host.region_bytes(2),a);}
    std::uint16_t shared(unsigned a){return word(host.region_bytes(3),a);}
    void set(unsigned a,std::uint16_t v){host.write_memory_word(2,a,v,0xffff);}
    bool frame(){return host.stage_select_frame(kController);}
    // Replay the original stage-clear writes: the 90-frame countdown at
    // $18(A5) (0xd858), addq.w #1,$c02 at 0xd862, then either the new-round
    // branch (addq $c00, $8002, $8006 at 0xd888) or the same-round branch
    // ($8006 at 0xd896), exactly as the routine decides.
    void clear_stage(){
        set(kController+0x18,0x5a);
        const auto advanced=host.stage_select_write(2,0xc02,static_cast<std::uint16_t>(ram(0xc02)+1),0xd862);
        set(0xc02,advanced);
        if(advanced%10==0){
            set(0xc00,static_cast<std::uint16_t>(ram(0xc00)+1));
            host.write_memory_word(3,0x38002,static_cast<std::uint16_t>(ram(0xc00)+1),0xffff);
            host.write_memory_word(3,0x38006,host.stage_select_write(3,0x38006,static_cast<std::uint16_t>(advanced+1),0xd888),0xffff);
        }else{
            host.write_memory_word(3,0x38006,host.stage_select_write(3,0x38006,static_cast<std::uint16_t>(advanced+1),0xd896),0xffff);
        }
        set(0xc16,2);
    }
};
}
int main(){try{
    { // Original same-round clear: stage 6 -> 7 of round 1, selector only.
        Board b(5,0);b.clear_stage();
        check(b.ram(0xc02)==6 && b.ram(0xc00)==0 && b.shared(0x38006)==7 && b.shared(0x38002)==0,"Original same-round clear changed");
    }
    { // Original round boundary: stage 10 of round 1 -> round 2, bank and selector.
        Board b(9,0);b.clear_stage();
        check(b.ram(0xc02)==10 && b.ram(0xc00)==1 && b.shared(0x38002)==2 && b.shared(0x38006)==11,"Original round-boundary clear changed");
    }
    { // Selecting during play zeroes the enemy count once, so the original
      // all-enemies-defeated clear fires; that clear is routed to Round 3
      // Stage 4 (index 23): bank 2, stage 23.
        Board b(5,0);b.host.set_start_stage(23);
        check(b.frame() && b.ram(0xc14)==0 && b.ram(0xc16)==0,"Play frame did not hand the clear to the original trigger");
        check(!b.frame(),"Enemy count was zeroed twice");
        b.clear_stage();
        check(b.ram(0xc02)==23,"Chosen stage index was not planted");
        check(b.ram(0xc00)==2 && b.shared(0x38002)==3,"World bank for the chosen round was not requested");
        check(b.shared(0x38006)==24,"CPU-A stage selector was not the chosen stage");
        check(b.ram(kController+0x18)==0x5a,"Forced clear lost the countdown that carries players over");
        check(b.host.start_stage()==-1 && b.host.take_start_stage_applied(),"Selection did not clear after use");
        b.set(0xc16,0);b.set(0xc14,9);b.clear_stage();
        check(b.ram(0xc02)==24 && b.ram(0xc00)==2 && b.shared(0x38006)==25 && !b.host.take_start_stage_applied(),"Selection applied twice");
    }
    { // A clear already under way carries the pending stage; the enemy count is left alone.
        Board b(5,0);b.set(0xc16,2);b.host.set_start_stage(12);
        check(b.frame() && b.ram(0xc16)==2 && b.ram(0xc14)==16,"Ongoing clear was disturbed");
        b.clear_stage();
        check(b.ram(0xc02)==12 && b.ram(0xc00)==1 && b.shared(0x38002)==2 && b.shared(0x38006)==13,"Pending stage was not carried by the ongoing clear");
    }
    { // Selecting Round 1 Stage 1 (index 0) from round 3 returns to bank 0.
        Board b(27,2);b.host.set_start_stage(0);b.clear_stage();
        check(b.ram(0xc02)==0 && b.ram(0xc00)==0 && b.shared(0x38002)==1 && b.shared(0x38006)==1,"Return to the first stage was not routed through bank 0");
    }
    { // A withdrawn selection leaves the original flow alone.
        Board b(5,0);b.host.set_start_stage(23);b.host.set_start_stage(-1);
        check(!b.frame() && b.ram(0xc14)==16,"Withdrawn selection still zeroed the enemy count");
        b.clear_stage();
        check(b.ram(0xc02)==6 && b.shared(0x38006)==7 && b.shared(0x38002)==0,"Withdrawn selection still changed the stage");
    }
    { // The attract demo (players marked in 0xc06) is never forced, and its
      // boot/attract handshake at 0xd390 never consumes the selection.
        Board b(1,0,7);b.host.set_start_stage(23);
        check(!b.frame() && b.ram(0xc14)==16 && b.host.start_stage()==23,"Attract demo stage was forced");
        check(b.host.stage_select_write(3,0x38006,2,0xd390)==2 && b.host.start_stage()==23,"Attract handshake consumed the selection");
    }
    std::cout<<"PASS: stage select hands one clear to the original all-enemies-defeated trigger and routes it through the new-round branch; boot, attract and later clears stay original\n";
}catch(const std::exception &e){std::cerr<<e.what()<<'\n';return 1;}}
