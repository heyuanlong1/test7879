package.cpath = "luaclib/?.so"
package.path = "lualib/?.lua"


local md5 = require "md5"

local m = {}

function m.create(nodes)
    local instance = {}

    
    local hashToNode = {}
    local hashCircle = {}
    
    local function getNodeHash(node)
        local result = {}
        local copyNum = 200
        for i=1, copyNum / 4 do
            local name = tostring(node).."_"..i
            local hash = md5.sumhexa(name)
            table.insert(result, tonumber("0x"..string.sub(hash, 1, 8)))
            table.insert(result, tonumber("0x"..string.sub(hash, 9, 16)))
            table.insert(result, tonumber("0x"..string.sub(hash, 17, 24)))
            table.insert(result, tonumber("0x"..string.sub(hash, 25, 32)))
        end
    
        return result
    end
    
    function instance:addNode(node)
        for i, hash in ipairs(getNodeHash(node)) do
            if  hashToNode[hash] and hashToNode[hash] ~= node then
                print(hash.."已经关联了节点"..hashToNode[hash]..", 待处理节点"..node)
            end
            hashToNode[hash] = node
            table.insert(hashCircle, hash)
        end
        table.sort(hashCircle)
    end
    
    function instance:removeNode(node)
        local hashs = getNodeHash(node)
        local set = {}
        for i, hash in ipairs(hashs) do
            if not hashToNode[hash] then
                error(hash.."没有关联节点")
            end
            hashToNode[hash] = nil
            set[hash] = true
        end
        local array = {}
        for i, oldHash in ipairs(hashCircle) do
            if not set[oldHash] then
                table.insert(array, oldHash)
            end
        end
        hashCircle = array
    end
    
    local function binarySearch(sorted, head, tail, value)
        assert(head <= tail)
        local mid = math.floor((head + tail) / 2)
        local midright = mid + 1
        
        if value >= sorted[mid] and value < sorted[midright] then
            return mid
        elseif value < sorted[mid] then
            return binarySearch(sorted, head, mid, value)
        elseif value >= sorted[midright] then
            return binarySearch(sorted, midright, tail, value)
        end
        assert("Illegal")
    end
    
    function instance:locate(key)
        local hash = tonumber("0x"..string.sub(md5.sumhexa(key), 1, 8))
        
        local index
        if hash < hashCircle[1] then
            index = #hashCircle
        elseif hash >= hashCircle[#hashCircle] then
            index = #hashCircle
        else
            index = binarySearch(hashCircle, 1, #hashCircle, hash)
        end
        
        return hashToNode[hashCircle[index]]    
    end
    
    for i, node in ipairs(nodes) do
        instance:addNode(node)
    end

    return instance
end



local testNode1 = {}
for i=1, 5 do
    table.insert(testNode1, i)
end
test1 = m.create( testNode1 )

local testNode2 = {}
for i=1, 13 do
    table.insert(testNode2, i)
end
test2 = m.create( testNode2 )





for i=1,20 do
    print(i.."->"..test1:locate(i))
end
print("")
for i=1,20 do
    print(i.."->"..test2:locate(i))
end
