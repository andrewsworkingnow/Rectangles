const assert=require('node:assert/strict'),E=require('../web/engine.js');
let r=E.rectangles('0,0,2,2\n1,1,2,2\n2,0,2,1');assert.equal(r.groups,2);assert.notEqual(r.colors[0],r.colors[1]);assert.equal(r.colors[0],r.colors[2]);assert.throws(()=>E.rectangles('0,0,-1,2'));
let s=E.schedule('0,7,2\n2,4,1\n4,1,3\n5,4,2','FCFS',2);assert.deepEqual(s.processes.map(p=>p.completion),[7,11,12,16]);assert.equal(s.averageWaiting,4.75);
assert.deepEqual(E.schedule('0,7,2\n2,4,1\n4,1,3\n5,4,2','SRTF',2).processes.map(p=>p.completion),[16,7,5,11]);
assert.deepEqual(E.schedule('0,4,1\n1,2,1','Round Robin',2).processes.map(p=>p.completion),[6,4]);
assert.equal(E.schedule('5,2,1','SJF',2).timeline[0].id,-1);
assert.deepEqual(E.schedule('0,4,2\n0,2,1','Priority',2).processes.map(p=>p.completion),[6,2]);assert.throws(()=>E.schedule('0,0,1','FCFS',2));
let b=E.banker(JSON.stringify({available:[3,3,2],allocation:[[0,1,0],[2,0,0],[3,0,2],[2,1,1],[0,0,2]],maximum:[[7,5,3],[3,2,2],[9,0,2],[2,2,2],[4,3,3]]}));assert.equal(b.safe,true);assert.equal(b.sequence.length,5);assert.equal(E.banker('{"available":[0],"allocation":[[1]],"maximum":[[2]]}').safe,false);assert.throws(()=>E.banker('{"available":[1],"allocation":[[2]],"maximum":[[1]]}'));
let p=E.polygons('[[[0,0],[4,0],[4,3],[0,3]]]','2,1');assert.equal(p.results[0].area,12);assert.equal(p.results[0].perimeter,14);assert.equal(p.results[0].probe,'Inside');assert.equal(E.polygons('[[[0,0],[4,0],[4,3],[0,3]]]','0,1').results[0].probe,'Boundary');assert.throws(()=>E.polygons('[[[0,0],[4,4],[0,4],[4,0]]]','1,1'));
const scenario=JSON.stringify({available:[3,3,2],allocation:[[0,1,0],[2,0,0],[3,0,2],[2,1,1],[0,0,2]],maximum:[[7,5,3],[3,2,2],[9,0,2],[2,2,2],[4,3,3]]});assert.equal(E.request(scenario,1,'1,0,2').granted,true);assert.equal(E.request(scenario,4,'3,3,0').granted,false);assert.equal(E.banker(scenario).safe,true);assert.throws(()=>E.request(scenario,1,'5,0,0'));assert.throws(()=>E.rectangles('0,,2,2,3'));
console.log('Algorithm checks passed: overlap/touching, scheduling/preemption/idle/ties, safety/requests/invalid matrices, geometry/boundaries/self-intersection.');

