function generateWave()
{
    var values = [];

    for(var i = 0; i < 5; i++)
    {
        values.push(rep.itemAt(i).enabled ? 1 : 0);
        values.push(amplRep.itemAt(i).value * 0.1);
        values.push(rep.itemAt(i).value);
    }

    return values;
}
