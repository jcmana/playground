// Global ZEN Automation Promise storage variable
// (each array row belongs to single promise resolution)
if (zen_automation_promise === undefined)
{
    zen_automation_promise = [];
}

zen_automation_promise.push({});
var current = zen_automation_promise.length - 1;
var self = this;

// Initialize current promise holder
zen_automation_promise[current]["done"] = false;
zen_automation_promise[current]["retv"] = null;
zen_automation_promise[current]["call"] = function()
{
    return /* promise returning call */;
};

// Register promise resolution callback
zen_automation_promise[current]["call"]().then(function(promise_value)
{
    zen_automation_promise[current]["done"] = true;
    zen_automation_promise[current]["retv"] = JSON.stringify(promise_value);
});

return current;
