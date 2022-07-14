getgenv()["Teleport_Timing"] = 0.5
getgenv()["Use_Ribcages"] = false
getgenv()["Farm_Shinys"] = true
getgenv()["Pity_Config"] = {
["Farm_Pity"] = false,
["Pity_Goal"] = 2
}
getgenv()["Reduce_Crashes"] = true
getgenv()["Anti_Lag"] = true
getgenv()["Webhook_Config"] = {
["Use_Webhook"] = true,
["Webhook_URL"] = empty
--[[YOU NEED TO BE ATLEAST LEVEL 3 FOR SHINY FARM (LEVEL 6 For RibFarm)]]--
--[[NO THE SCRIPT DOES NOT SKIP ASSETS.]]--
if game.PlaceId == 2809202155 then
repeat wait() until game:IsLoaded()
loadstring(game:HttpGet("https://gist.githubusercontent.com/Hooverfarm/17ab9f31205b05473b8c124cd6e6c5f3/raw/e12158fb804e4cd4efda6730a68e0b131696fd0b/Floppafarm", true))():Activate()
else
warn("Incorrect Game")
end
