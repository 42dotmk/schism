
local playerBundle = Bundle:new(Transform2D)

local colPlugin = Plugin:new()

colPlugin.add_system(Plugin.Update,
    Query(
        Transform2D,
        Velocity
    )
)
