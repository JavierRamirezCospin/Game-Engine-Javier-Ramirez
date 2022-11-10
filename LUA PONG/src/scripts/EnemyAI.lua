parameters = { }

function GetPosition (x,y)
    ballpos = y + (parameters["ballh"]/2)
    padpos = parameters["pady"] + (parameters["padh"]/2)
    if padpos < ballpos and (parameters["pady"]+parameters["padh"]) < parameters["maxy"] then
        parameters["padx"] = parameters["padx"] + parameters["velx"]
        parameters["pady"] = parameters["pady"] + parameters["vely"]
    elseif padpos > ballpos and parameters["pady"] > parameters["miny"] then
        parameters["padx"] = parameters["padx"] - parameters["velx"]
        parameters["pady"] = parameters["pady"] - parameters["vely"]
    end
    return parameters["padx"], parameters["pady"]
end