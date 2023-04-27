function mat2str(mat)
    local res=""
    res=res.."{"
    for i=1,#mat do
            res=res.."{"
            for j=1,#mat[i] do
                res=res..mat[i][j]..", "
            end
            res=res.."}, "
    end
    res=res.."}"
    return res
end

a=glm.new_mat4()
print(mat2str(a:to_array()))

