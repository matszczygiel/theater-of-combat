#include "kircholm/kirch_retreat.h"

#include <imgui.h>

namespace kirch {
RetreatSystem::RetreatSystem(SystemKircholm* system) noexcept
    : ComponentSystemKircholm{system} {}

void RetreatSystem::set_results(const std::vector<DirectFightResult>& results) {
    _results = results;
}

bool RetreatSystem::is_done() { return false; }

void RetreatSystem::process_retreats() {
    /*  if (_request_retreat)
          return;
      if (!ImGui::Begin("Fight results", nullptr)) {
          ImGui::End();
          return;
      }

      int counter = 0;
      for (const auto& res : _current_results) {
          ImGui::BulletText("Result %d", counter);
          if (ImGui::Button("Retreat", ImVec2(200, 100))) {
              _request_retreat = counter++;
              fetch_handled_retreat();
          }
          ImGui::Separator();
      }
      ImGui::End();
  */
}
}  // namespace kirch