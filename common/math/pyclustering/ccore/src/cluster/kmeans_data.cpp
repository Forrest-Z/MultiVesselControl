/**
 *
 * @authors Andrei Novikov (pyclustering@yandex.ru)
 * @date 2014-2019
 * @copyright GNU Public License
 *
 * GNU_PUBLIC_LICENSE
 *   pyclustering is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   pyclustering is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <pyclustering/cluster/kmeans_data.hpp>

namespace pyclustering {

namespace clst {

kmeans_data::kmeans_data(const bool p_iteration_observe)
    : m_observed(p_iteration_observe) {}

}  // namespace clst

}  // namespace pyclustering