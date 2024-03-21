/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MaterialManager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIALMANAGER_HPP
#define MATERIALMANAGER_HPP

#include "Texture.hpp"
#include "define.hpp"

class MaterialManager {
public:
    bool loadMaterials(const string& mtlFilePath);
    const Material* getMaterial(const string& name) const;

private:
    unordered_map<string, Material> materials;
    void parseMtlFile(ifstream& file);
};

#endif
