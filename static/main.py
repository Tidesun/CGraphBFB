import random
class Segment:
    def __init__(self,name,dir):
        self.name = name
        self.dir = dir
    def toString(self):
        return str(self.name) + self.dir
def reverseDir(dir):
    if dir == '+':
        return '-'
    else:
        return '+'
def simulate(source,sink,simDepth):
    segs = []
    juncDic = {}
    segDic = {}
    currDir = '+'
    for i in range(source,sink+1):
        segs.append(Segment(i,currDir))
    for _ in range(simDepth):
        bkPos = random.randint(0,len(segs)-2)
        republicatedSegs = segs[bkPos:len(segs)][::-1]
        for seg in republicatedSegs:
            newSeg = Segment(seg.name,reverseDir(seg.dir))
            segs.append(newSeg)
    del segs[random.randint(0,len(segs)-2)]
    prevSeg = segs[0]
    segDic[segs[0].name] = 1
    for seg in segs[1:]:
        key = juncHash(prevSeg.name,prevSeg.dir,seg.name,seg.dir)
        if key in juncDic:
            juncDic[key] += 1
        else:
            juncDic[key] = 1

        if seg.name in segDic:
            segDic[seg.name] += 1
        else:
            segDic[seg.name] = 1
        prevSeg = seg
    print(' '.join([x.toString() for x in segs]))
    print(len(segs))
    return getResultStr(source,sink,segDic,juncDic)

def getResultStr(source,sink,segDic,juncDic):
    output = ''
    chrPos = [28460000]
    chrPos += random.sample(range(28460001,33500000), sink-source)
    chrPos.append(33500000)
    chrPos.sort()
    for i in range(source,sink+1):
        output += getSegment(i,chrPos[i-1],chrPos[i],segDic[i])
    for key in juncDic:
        output += getJunction(key[0],key[1],key[2],key[3],juncDic[key])
    return output

def getSegment(name,startPos,endPos,copyNum):
    # * random.randint(101,102) * 0.01
    return 'SEG H:{}:chr6:{}:{} {}\n'.format(name,startPos,endPos,copyNum * 2 )
def getJunction(seg1,dir1, seg2, dir2,copyNum):
    return 'JUNC H:{}:{} H:{}:{} {}\n'.format(seg1,dir1, seg2, dir2,copyNum * 2)
def juncHash(seg1,dir1, seg2, dir2):
    if (seg1>seg2):
        return (seg2,reverseDir(dir2),seg1,reverseDir(seg1))
    else:
        return (seg1,dir1, seg2, dir2)
with open('sim.lh','w+') as f:
    source = 1
    sink = 20
    #645.453313600533
    f.write('''SAMPLE sim_1\nAVG_DP 4\nPURITY 1\nAVG_PLOIDY 2\nPLOIDY 2m1\n''')
    f.write('SOURCE H:{}\nSINK H:{}\n'.format(source,sink))
    f.write(simulate(source,sink,5))
        